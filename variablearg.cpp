#include "variablearg.hpp"
#include "variablesymbol.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
    
VariableArg::VariableArg(VariableSymbol* var) : var(var) { }

void VariableArg::gen(const Block& block, CodeObject& code_obj) const
{
    if ( var -> isField() )
    {
        auto sym = static_cast<VariableSymbol*>(block.scope.resolve("this"));
        auto struc_sym = static_cast<const StructSymbol*>(sym -> getType().unqualified());

        block.alloc.remember(sym);

        code_obj.emit("mov rax, [rbp - " + std::to_string(block.alloc.addressOf(sym)) + "]");
        if ( int addr = struc_sym -> offsetOf(var) )
            code_obj.emit("lea rax, [rax + " + std::to_string(addr) + "]");
    }
    else if ( var-> getType().isReference() )
    {
        code_obj.emit("mov rax, [rbp - " + std::to_string(block.alloc.addressOf(var)) + "]");
    }
    else
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(var)) + "]");
    }
}

std::string VariableArg::toString() const { return var -> getName(); }
const Type* VariableArg::type() const     { return var -> getType().base(); }
