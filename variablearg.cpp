#include "variablearg.hpp"
#include "variablesymbol.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"

#include "logger.hpp"

VariableArg::VariableArg(const VariableSymbol* var) : var_(var) { }

void VariableArg::gen(const Block& block, CodeObject& code_obj) const
{
    if ( var_ -> isField() )
    {
        auto sym = static_cast<const VariableSymbol*>(block.scope() -> resolve("this"));
        auto struc_sym = static_cast<const StructSymbol*>(sym -> getType().unqualified());

        block.allocate(sym);

        code_obj.emit("mov rax, [rbp - " + std::to_string(block.addressOf(sym)) + "]");
        if ( int addr = struc_sym -> offsetOf(var_) )
            code_obj.emit("lea rax, [rax + " + std::to_string(addr) + "]");
    }
    else if ( var_ -> getType().isReference() )
    {
        code_obj.emit("mov rax, [rbp - " + std::to_string(block.addressOf(var_)) + "]");
    }
    else
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.addressOf(var_)) + "]");
    }
}

std::string VariableArg::toString() const { return var_ -> getName(); }
const Type* VariableArg::type() const     { return var_ -> getType().base(); }
    
const VariableSymbol* VariableArg::var() const { return var_; }
