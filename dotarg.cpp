#include "dotarg.hpp"
#include "variablesymbol.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "temporaryarg.hpp"
#include "variablearg.hpp"
#include "logger.hpp"

DotArg::DotArg(Argument expr, int offset, const VarSymbol* member) : expr_(expr), offset_(offset), member_(member) { }

void DotArg::gen(const Block& block, CodeObject& code_obj) const
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(expr_.get()) )
    {
        if ( temp -> type() -> isReference() )
        {            
            code_obj.emit("mov rax, [rbp - " + std::to_string(block.addressOf(temp -> command())) + "]");
            code_obj.emit("lea rax, [rax + " + std::to_string(offset_) + "]");
        }
        else
        {
            code_obj.emit("lea rax, [rbp - " + std::to_string(block.addressOf(temp -> command()) - offset_) + "]");
        }
    }    
    else if ( auto var = dynamic_cast<VariableArg*>(expr_.get()) )
    {
        if ( var -> type() -> isReference() )
        {            
            code_obj.emit("mov rax, [rbp - " + std::to_string(block.addressOf(var -> var())) + "]");
            code_obj.emit("lea rax, [rax + " + std::to_string(offset_) + "]");
        }
        else
        {
            code_obj.emit("lea rax, [rbp - " + std::to_string(block.addressOf(var -> var()) - offset_) + "]");
        }
    }
    else if ( auto dot = dynamic_cast<DotArg*>(expr_.get()) )
    {
        dot -> gen(block, code_obj);

        if ( dot -> type() -> isReference() ) {
            code_obj.emit("mov rax, [rax]");
        }

        code_obj.emit("lea rax, [rax + " + std::to_string(offset_) + "]");
    }
    else throw "";
}

std::string DotArg::toString() const { return expr_ -> toString() + "." + member_ -> getName(); }
const Type* DotArg::type() const     { return member_ -> typeOf().base(); }
    
Arg* DotArg::expr() const { return expr_.get(); }
int DotArg::offset() const { return offset_; }
const VarSymbol* DotArg::member() const { return member_; }

