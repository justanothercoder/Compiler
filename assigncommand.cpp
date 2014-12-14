#include "assigncommand.hpp"
#include "codeobject.hpp"
#include "type.hpp"
#include "builtins.hpp"

#include "logger.hpp"

AssignCommand::AssignCommand(Arg* lhs, Arg* rhs, bool is_char) : lhs(lhs), rhs(rhs), is_char(is_char)
{

}

void AssignCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( is_char )
    {
        rhs -> gen(block, code_obj);
        if ( rhs -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        code_obj.emit("mov cl, byte [rax]");

        lhs -> gen(block, code_obj);
        if ( lhs -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        code_obj.emit("mov byte [rax], cl");
    }
    else
    {
        rhs -> gen(block, code_obj);
        if ( rhs -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        
        if ( rhs -> type() -> removeRef() == BuiltIns::char_type )
        {
            code_obj.emit("xor rbx, rbx");
            code_obj.emit("mov bl, byte [rax]");
        }
        else
            code_obj.emit("mov rbx, [rax]");

        lhs -> gen(block, code_obj);
        if ( lhs -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        code_obj.emit("mov [rax], rbx");
    }
}

std::string AssignCommand::toString() const
{
    return lhs -> toString() + " = "  + rhs -> toString();
}

bool AssignCommand::isExpr() const
{
    return false;
}
    
const Type* AssignCommand::type() const 
{
    return nullptr;
}
