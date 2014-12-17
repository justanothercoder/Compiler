#include "dotcommand.hpp"
#include "structsymbol.hpp"
#include "variablearg.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "temporaryarg.hpp"
#include "tempallocator.hpp"
#include "commandvisitor.hpp"

DotCommand::DotCommand(Arg* expr, int offset, VariableSymbol* member) : expr(expr), offset(offset), member(member)
{

}

void DotCommand::gen(const Block& block, CodeObject& code_obj) const
{
    auto base_type = expr -> type();

    int arg_addr;
    if ( dynamic_cast<VariableArg*>(expr) )
    {
        if ( base_type -> isReference() )
        {
            expr -> gen(block, code_obj);

            code_obj.emit("mov rax, [rax + " + std::to_string(offset) + "]");
            code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");

            return;
        }

        auto base_var = dynamic_cast<VariableArg*>(expr) -> var;

        if ( base_var -> isField() )
        {
            expr -> gen(block, code_obj);

            code_obj.emit("mov rax, [rax + " + std::to_string(offset) + "]");
            code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");

            return;
        }
    }
}

std::string DotCommand::toString() const
{
    return expr -> toString() + "." + member -> getName();
}

bool DotCommand::isExpr() const
{
    return true;
}

const Type* DotCommand::type() const
{
    return member -> getType().base();
}

void DotCommand::accept(CommandVisitor* visitor)
{
    visitor -> visit(this);
}
