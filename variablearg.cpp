#include "variablearg.hpp"
#include "block.hpp"
#include "scope.hpp"
#include "codeobject.hpp"
#include "varsymbol.hpp"
#include "objecttype.hpp"

#include "logger.hpp"

VariableArg::VariableArg(const VarSymbol* var) : var_(var) { }

void VariableArg::gen(const Block& block, CodeObject& code_obj) const
{
    if ( var_ -> isField() )
    {
        auto sym = block.scope() -> resolveVariable("this");
        auto struc_sym = static_cast<const ObjectType*>(sym -> typeOf().unqualified());

        block.allocate(sym);

        code_obj.emit("mov rax, [rbp - " + std::to_string(block.addressOf(sym)) + "]");
        if ( int addr = struc_sym -> offsetOf(var_ -> getName()) )
            code_obj.emit("lea rax, [rax + " + std::to_string(addr) + "]");
    }
    else if ( var_ -> typeOf().isReference() )
    {
        code_obj.emit("mov rax, [rbp - " + std::to_string(block.addressOf(var_)) + "]");
    }
    else
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.addressOf(var_)) + "]");
    }
}

std::string VariableArg::toString() const { return var_ -> getName(); }
const Type* VariableArg::type() const     { return var_ -> typeOf().base(); }
    
const VarSymbol* VariableArg::var() const { return var_; }
