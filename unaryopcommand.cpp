#include "unaryopcommand.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "scope.hpp"
#include "globalconfig.hpp"
#include "builtins.hpp"
#include "pointertype.hpp"
#include "typefactory.hpp"
#include "commandvisitor.hpp"
    
UnaryOpCommand::UnaryOpCommand(AddrOp op, Arg* expr) : op(op), expr(expr) { } 
UnaryOpCommand::UnaryOpCommand(UnaryOp op, Arg* expr) : op(op), expr(expr) { }

void UnaryOpCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( op.which() == 0 )
    {
        if ( boost::get<AddrOp>(op) == AddrOp::DEREF )
        {
            expr -> gen(block, code_obj);

            //if ( expr -> getType() -> isReference() )
            //  code_obj.emit("mov rax, [rax]");
            
            auto addr = "[rbp - " + std::to_string(block.alloc.addressOf(this)) + "]";
            
            code_obj.emit("mov " + addr + ", rax");
            code_obj.emit("lea rax, " + addr);
        }
        else
        {
            auto addr = "[rbp - " + std::to_string(block.alloc.addressOf(this)) + "]";

            expr -> gen(block, code_obj);

            code_obj.emit("mov " + addr + ", rax");
            code_obj.emit("lea rax, " + addr);
        }
    }
}

std::string UnaryOpCommand::toString() const
{
    if ( op.which() == 0 ) 
    {
        if ( boost::get<AddrOp>(op) == AddrOp::REF )
            return "&" + expr -> toString();
        else
            return "*" + expr -> toString();
    }
    else
        return expr -> toString();
}

bool UnaryOpCommand::isExpr() const
{
    return true;
}

const Type* UnaryOpCommand::type() const
{
    if ( op.which() == 1 )
        return BuiltIns::int_type;

    if ( boost::get<AddrOp>(op) == AddrOp::DEREF )
        return TypeFactory::getReference(static_cast<const PointerType*>(expr -> type()) -> pointedType());
    else
        return TypeFactory::getPointer(VariableType(expr -> type()).unqualified());
}

void UnaryOpCommand::accept(CommandVisitor* visitor)
{
    visitor -> visit(this);
}