#include "dotcommand.hpp"
#include "structsymbol.hpp"
#include "variablearg.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "temporaryarg.hpp"
#include "tempallocator.hpp"

DotCommand::DotCommand(Arg* expr, int offset, VariableSymbol* member) : expr(expr), offset(offset), member(member)
{

}

void DotCommand::gen(const Block& block, CodeObject& code_obj) const
{
    auto base_type = expr -> type();
    auto base_sym = static_cast<const StructSymbol*>(base_type -> removeRef());

    int arg_addr;
    if ( dynamic_cast<VariableArg*>(expr) )
    {
        if ( base_type -> isReference() )
        {
            expr -> gen(block, code_obj);

            block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
            block.scope.tempAlloc().claim(GlobalConfig::int_size);

            code_obj.emit("mov rax, [rax + " + std::to_string(base_sym -> varAlloc().getAddress(member)) + "]");
            code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");

            return;
        }

        auto base_var = dynamic_cast<VariableArg*>(expr) -> var;

        if ( base_var -> isField() )
        {
            expr -> gen(block, code_obj);

            block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
            block.scope.tempAlloc().claim(GlobalConfig::int_size);

            code_obj.emit("mov rax, [rax + " + std::to_string(base_sym -> varAlloc().getAddress(member)) + "]");
            code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");

            return;
        }

        arg_addr = block.scope.varAlloc().getAddress(base_var);
    }
    else if ( dynamic_cast<TemporaryArg*>(expr) )
    {
        arg_addr = block.alloc.addressOf(static_cast<TemporaryArg*>(expr) -> command);
    }
    else
    {
        arg_addr = 0;
    }

    block.alloc.remember(this, arg_addr - base_sym -> varAlloc().getAddress(member));
}

std::string DotCommand::toString() const
{
    return expr -> toString() + "." + member -> getName();
}
