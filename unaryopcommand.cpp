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
    
UnaryOpCommand::UnaryOpCommand(AddrOp  op, Argument expr) : op_(op), expr_(expr) { } 
UnaryOpCommand::UnaryOpCommand(UnaryOp op, Argument expr) : op_(op), expr_(expr) { }

void UnaryOpCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( op_.which() == 0 )
    {
        if ( boost::get<AddrOp>(op_) == AddrOp::DEREF )
        {
            expr_ -> gen(block, code_obj);

            auto addr = "[rbp - " + std::to_string(block.addressOf(this)) + "]";
            
            code_obj.emit("mov " + addr + ", rax");
            code_obj.emit("lea rax, " + addr);
        }
        else
        {
            auto addr = "[rbp - " + std::to_string(block.addressOf(this)) + "]";

            expr_ -> gen(block, code_obj);

            code_obj.emit("mov " + addr + ", rax");
            code_obj.emit("lea rax, " + addr);
        }
    }
}

std::string UnaryOpCommand::toString() const
{
    if ( op_.which() == 0 ) 
    {
        if ( boost::get<AddrOp>(op_) == AddrOp::REF )
            return "&" + expr_ -> toString();
        else
            return "*" + expr_ -> toString();
    }
    else
        return expr_ -> toString();
}

bool UnaryOpCommand::isExpr() const { return true; }

const Type* UnaryOpCommand::type() const
{
    if ( op_.which() == 1 )
        return BuiltIns::int_type;

    if ( boost::get<AddrOp>(op_) == AddrOp::DEREF )
        return TypeFactory::getReference(static_cast<const PointerType*>(expr_ -> type()) -> pointedType());
    else
        return TypeFactory::getPointer(VariableType(expr_ -> type()).unqualified());
}

void UnaryOpCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }

Arg* UnaryOpCommand::expr() { return expr_.get(); }
boost::variant<AddrOp, UnaryOp> UnaryOpCommand::op() { return op_; }

