#include "optimizer.hpp"
#include "globaltable.hpp"
#include "scope.hpp"

#include "substituteargvisitor.hpp"
#include "checkforusevisitor.hpp"

#include "binaryopcommand.hpp"
#include "numberarg.hpp"
#include "temporaryarg.hpp"
#include "dotarg.hpp"
#include "callcommand.hpp"

Optimizer::Optimizer(ThreeAddressCode& code) : code(code)
{

}

void Optimizer::optimize()
{
    constantPropagation();
    eliminateUnusedTemporaries();
}

void Optimizer::constantPropagation()
{
    for ( auto& block : code.blocks )
    {
        for ( auto it = std::begin(block -> code()); it != std::end(block -> code()); ++it )
        {
            int command_id = *it;
            auto command = block -> commandById(command_id);
    
            if ( auto com = dynamic_cast<BinaryOpCommand*>(command) )
            {
                auto lhs = dynamic_cast<NumberArg*>(com -> lhs());
                auto rhs = dynamic_cast<NumberArg*>(com -> rhs());

                if ( lhs && rhs )
                {
                    int n1 = lhs -> value();
                    int n2 = rhs -> value();

                    int n3;

                    switch ( com -> op() )
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
    
                    auto num_arg = std::make_shared<NumberArg>(n3);

                    SubstituteArgVisitor substitutor(std::bind([](const Argument& arg1, Command* command, const Argument& arg2) -> Argument { 
                        if ( auto temp = dynamic_cast<const TemporaryArg*>(arg1.get()) )
                        {
                            if ( temp -> command() == command )
                                return arg2;
                        }
                        else if ( auto dot = dynamic_cast<const DotArg*>(arg1.get()) )
                        {
                            if ( dynamic_cast<TemporaryArg*>(dot -> expr()) )
                                return std::make_shared<DotArg>(arg2, dot -> offset(), dot -> member());
                        }
                        return arg1;
                    }, std::placeholders::_1, com, num_arg));

                    for ( ++it2; it2 != std::end(block -> code()); ++it2 )
                    {
                        int next_command_id = *it2;
                        auto next_command = block -> commandById(next_command_id);

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

        for ( auto command_id : block -> code() )
        {
            auto command = block -> commandById(command_id);
            command -> accept(&checker);
        }

        std::remove_if(std::begin(block -> code()), std::end(block -> code()), [&] (int command_id) 
        {
            return !checker.isUsed(block -> commandById(command_id));
        });
    }
    
}
