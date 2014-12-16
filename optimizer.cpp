#include "optimizer.hpp"
#include "globaltable.hpp"
#include "scope.hpp"

#include "substituteargvisitor.hpp"
#include "checkforusevisitor.hpp"

#include "binaryopcommand.hpp"
#include "numberarg.hpp"
#include "temporaryarg.hpp"
#include "callcommand.hpp"

Optimizer::Optimizer(ThreeAddressCode& code) : code(code)
{

}

void Optimizer::optimize()
{
    constantPropagation();
    eliminateUnusedTemporaries();
    callInlining();
}

void Optimizer::constantPropagation()
{
    for ( auto& block : code.blocks )
    {
        for ( auto it = std::begin(block -> code); it != std::end(block -> code); ++it )
        {
            int command_id = *it;
            auto& command = block -> commands[command_id];
    
            if ( auto com = dynamic_cast<BinaryOpCommand*>(command) )
            {
                auto lhs = dynamic_cast<NumberArg*>(com -> lhs);
                auto rhs = dynamic_cast<NumberArg*>(com -> rhs);

                if ( lhs && rhs )
                {
                    int n1 = lhs -> value();
                    int n2 = rhs -> value();

                    int n3;

                    switch ( com -> op )
                    {
                        case BinaryOp::PLUS : n3 = n1 + n2; break;
                        case BinaryOp::MINUS: n3 = n1 - n2; break;
                        case BinaryOp::MUL  : n3 = n1 * n2; break;
                        case BinaryOp::DIV  : n3 = n1 / n2; break;                                              
                        case BinaryOp::MOD  : n3 = n1 % n2; break;                                              
                        default: throw std::logic_error("internal error.");
                    }

                    code.addConst(n3);
                    
                    auto it2 = it;
    
                    SubstituteArgVisitor substitutor(std::bind([](Arg* arg1, Command* command, Arg* arg2) { 
                        if ( dynamic_cast<TemporaryArg*>(arg1) )
                        {
                            if ( static_cast<TemporaryArg*>(arg1) -> command == command )
                                return arg2;
                        }
                        return arg1;
                    }, std::placeholders::_1, com, new NumberArg(n3)));

                    for ( ++it2; it2 != std::end(block -> code); ++it2 )
                    {
                        int next_command_id = *it2;
                        auto& next_command = block -> commands[next_command_id];

                        next_command -> accept(&substitutor);
                    }
                }
            }
        }
    }
    
}

void Optimizer::eliminateUnusedTemporaries()
{
    for ( auto& block : code.blocks )
    {
        CheckForUseVisitor checker;

        for ( auto command_id : block -> code )
        {
            auto command = block -> commands[command_id];
            command -> accept(&checker);
        }

        for ( auto it = std::begin(block -> code); it != std::end(block -> code); )
        {
            int  command_id = *it;
            auto command    = block -> commands[command_id];

            if ( !checker.isUsed(command) )
                it = block -> code.erase(it);
            else
                ++it;
        }
    }
    
}

void Optimizer::callInlining()
{
    for ( auto& block : code.blocks )
    {
        for ( auto it = std::begin(block -> code); it != std::end(block -> code); ++it )
        {
            int  command_id = *it;
            auto command    = block -> commands[command_id];
            
            if ( auto call_command = dynamic_cast<CallCommand*>(command) )
            {
                if ( block -> table.function_blocks.count(call_command -> function) )
                    call_command -> setInlineCall();
            }
        }
    }
}
