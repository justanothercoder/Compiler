#include "dotarg.hpp"
#include "variablesymbol.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "temporaryarg.hpp"
#include "variablearg.hpp"
#include "logger.hpp"

DotArg::DotArg(Arg* expr, int offset, VariableSymbol* member) : expr(expr), offset(offset), member(member) { }

void DotArg::gen(const Block& block, CodeObject& code_obj) const
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(expr) )
    {
        if ( temp -> type() -> isReference() )
        {            
            code_obj.emit("mov rax, [rbp - " + std::to_string(block.alloc.addressOf(temp -> command)) + "]");
            code_obj.emit("lea rax, [rax + " + std::to_string(offset) + "]");
        }
        else
        {
            code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(temp -> command) - offset) + "]");
        }
    }    
    else if ( auto var = dynamic_cast<VariableArg*>(expr) )
    {
        if ( var -> type() -> isReference() )
        {            
            code_obj.emit("mov rax, [rbp - " + std::to_string(block.alloc.addressOf(var -> var)) + "]");
            code_obj.emit("lea rax, [rax + " + std::to_string(offset) + "]");
        }
        else
        {
            code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(var -> var) - offset) + "]");
        }
    }
    else if ( auto dot = dynamic_cast<DotArg*>(expr) )
    {
        dot -> gen(block, code_obj);

        if ( dot -> type() -> isReference() ) {
            code_obj.emit("mov rax, [rax]");
        }

        code_obj.emit("lea rax, [rax + " + std::to_string(offset) + "]");
    }
    else throw "";
}

std::string DotArg::toString() const { return expr -> toString() + "." + member -> getName(); }
const Type* DotArg::type() const     { return member -> getType().base(); }
