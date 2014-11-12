#include "optimizer.hpp"
#include "globaltable.hpp"
#include "scope.hpp"

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
        for ( auto it = std::begin(block.code); it != std::end(block.code); ++it )
        {
            int command_id = *it;
            auto& command = block.commands[command_id];

            if ( command.arg1.type == IdType::NUMBER && command.arg2.type == IdType::NUMBER )
            {
                int n1 = code.getConstFromId(command.arg1.id);
                int n2 = code.getConstFromId(command.arg2.id);

                int n3;

                switch ( command.op )
                {
                case SSAOp::PLUS : n3 = n1 + n2; break;
                case SSAOp::MINUS: n3 = n1 - n2; break;
                case SSAOp::MUL  : n3 = n1 * n2; break;
                case SSAOp::DIV  : n3 = n1 / n2; break;
                case SSAOp::MOD  : n3 = n1 % n2; break;
                default:
                    throw std::logic_error("internal error.");
                }

                code.addConst(n3);

                auto it2 = it;

                for ( ++it2; it2 != std::end(block.code); ++it2 )
                {
                    int next_command_id = *it2;
                    auto& next_command = block.commands[next_command_id];

                    if ( next_command.arg1.type == IdType::TEMP && next_command.arg1.id == command_id )
                    {
                        next_command.arg1.type = IdType::NUMBER;
                        next_command.arg1.id   = code.getConstId(n3);
                    }
                    if ( next_command.arg2.type == IdType::TEMP && next_command.arg2.id == command_id )
                    {
                        next_command.arg2.type = IdType::NUMBER;
                        next_command.arg2.id   = code.getConstId(n3);
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
        std::vector<int> is_used(block.code.size(), 0);

        for ( auto command_id : block.code )
        {
            auto& command = block.commands[command_id];

            if ( !command.isExpr() || command.op == SSAOp::CALL )
                is_used[command_id] = 1;

            if ( command.arg1.type == IdType::TEMP )
                is_used[command.arg1.id] = 1;

            if ( command.arg2.type == IdType::TEMP )
                is_used[command.arg2.id] = 1;
        }

        int i = 0;
        for ( auto it = std::begin(block.code); it != std::end(block.code); ++i )
        {
            int command_id = *it;

            if ( !is_used[command_id] )
                it = block.code.erase(it);
            else
                ++it;
        }
    }
}