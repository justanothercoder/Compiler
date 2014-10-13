#include "optimizer.hpp"

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
    for ( auto block : code.blocks )
    {
        for ( auto it = std::begin(block.code); it != std::end(block.code); ++it )
        {
            int command_id = *it;
//            auto& command = code.commands[command_id];
            auto& command = block.commands[command_id];

            if ( command.arg1.type == IdType::NUMBER && command.arg2.type == IdType::NUMBER )
            {
                int n1 = GlobalHelper::id_to_num[command.arg1.id];
                int n2 = GlobalHelper::id_to_num[command.arg2.id];

                int n3;

                switch ( command.op )
                {
                    case SSAOp::PLUS : n3 = n1 + n2; break;
                    case SSAOp::MINUS: n3 = n1 - n2; break;
                    case SSAOp::MUL  : n3 = n1 * n2; break;
                    case SSAOp::DIV  : n3 = n1 / n2; break;
                    case SSAOp::MOD  : n3 = n1 % n2; break;
                    default: throw std::logic_error("internal error.");
                } 
               
                GlobalHelper::addConst(n3);
        
                auto it2 = it;          

                for ( ++it2; it2 != std::end(block.code); ++it2 )            
                {
                    int next_command_id = *it2;
//                    auto& command = code.commands[next_command_id];
                    auto& command = block.commands[next_command_id];

                    if ( command.arg1.type == IdType::TEMP && command.arg1.id == command_id )
                    {
                        command.arg1.type = IdType::NUMBER;
                        command.arg1.id   = GlobalHelper::const_num_id[n3];
                    }
                    if ( command.arg2.type == IdType::TEMP && command.arg2.id == command_id )
                    {
                        command.arg2.type = IdType::NUMBER;
                        command.arg2.id   = GlobalHelper::const_num_id[n3];
                    }
                } 
            }
        }
    }
}

void Optimizer::eliminateUnusedTemporaries()
{
    for ( auto block : code.blocks )
    {
        std::vector<int> is_used(block.code.size(), 0);

        for ( auto command_id : block.code )
        {
//            auto& command = code.commands[command_id];
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
