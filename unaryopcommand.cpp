#include "unaryopcommand.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "scope.hpp"
#include "tempallocator.hpp"
#include "globalconfig.hpp"
    
UnaryOpCommand::UnaryOpCommand(AddrOp op, Arg* expr) : op(op), expr(expr)
{

}

UnaryOpCommand::UnaryOpCommand(UnaryOp op, Arg* expr) : op(op), expr(expr)
{

}

void UnaryOpCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( op.which() == 0 )
    {
        if ( boost::get<AddrOp>(op) == AddrOp::DEREF )
        {
            expr -> gen(block, code_obj);

            //if ( expr -> getType() -> isReference() )
            //  code_obj.emit("mov rax, [rax]");

            code_obj.emit("mov rax, [rax]");
        }
        else
        {
            block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
            block.scope.tempAlloc().claim(GlobalConfig::int_size);
            
            auto addr = "[rbp - " + std::to_string(block.alloc.addressOf(this)) + "]";

            expr -> gen(block, code_obj);

            code_obj.emit("mov " + addr + ", rax");
            code_obj.emit("lea rax, " + addr);
            return;
        }
    }
}

std::string UnaryOpCommand::toString() const
{
    if ( op.which() == 0 ) return "*" + expr -> toString();
    else                   return "&" + expr -> toString();
}
