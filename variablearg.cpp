#include "variablearg.hpp"
#include "variablesymbol.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
    
VariableArg::VariableArg(VariableSymbol* var) : var(var)
{

}

void VariableArg::gen(const Block& block, CodeObject& code_obj) const
{
    if ( var -> isField() )
    {
        auto sym = static_cast<VariableSymbol*>(block.scope.resolve("this"));
        auto struc_scope = static_cast<const StructSymbol*>(sym -> getType().unqualified());

        code_obj.emit("mov rax, [rbp - " + std::to_string(block.scope.varAlloc().getAddress(sym)) + "]");
        if ( struc_scope -> varAlloc().getAddress(var) != 0 )
            code_obj.emit("lea rax, [rax + " + std::to_string(struc_scope -> varAlloc().getAddress(var)) + "]");
    }
    else if ( var-> getType().isReference() )
    {
        code_obj.emit("mov rax, [rbp - " + std::to_string(block.scope.varAlloc().getAddress(var)) + "]");
    }
    else
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.scope.varAlloc().getAddress(var)) + "]");
    }
}

std::string VariableArg::toString() const
{
    return var -> getName();
}
