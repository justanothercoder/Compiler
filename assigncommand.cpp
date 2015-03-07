#include "assigncommand.hpp"
#include "codeobject.hpp"
#include "type.hpp"
#include "builtins.hpp"
#include "commandvisitor.hpp"

AssignCommand::AssignCommand(Argument lhs, Argument rhs, bool is_char) : lhs_(lhs), rhs_(rhs), is_char(is_char) { }

void AssignCommand::gen(const Block& block, CodeObject& code_obj) const
{   
    code_obj.comment("Assigning " + lhs_ -> toString() + " to " + rhs_ -> toString());

    if ( is_char )
    {
        rhs_ -> gen(block, code_obj);
        if ( rhs_ -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        code_obj.emit("mov cl, byte [rax]");

        lhs_ -> gen(block, code_obj);
        if ( lhs_ -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        code_obj.emit("mov byte [rax], cl");
    }
    else
    {
        rhs_ -> gen(block, code_obj);
        if ( rhs_ -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        
        if ( isCharType(rhs_ -> type() -> removeRef()) )
        {
            code_obj.emit("xor rbx, rbx");
            code_obj.emit("mov bl, byte [rax]");
        }
        else
            code_obj.emit("mov rbx, [rax]");

        lhs_ -> gen(block, code_obj);
        if ( lhs_ -> type() -> isReference() )
            code_obj.emit("mov rax, [rax]");
        code_obj.emit("mov [rax], rbx");
    }
}

std::string AssignCommand::toString() const { return lhs_ -> toString() + " = "  + rhs_ -> toString(); }

bool AssignCommand::isExpr() const { return false; } 
const Type* AssignCommand::type() const { return nullptr; }

void AssignCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }

Arg* AssignCommand::lhs() { return lhs_.get(); }
Arg* AssignCommand::rhs() { return rhs_.get(); }
bool AssignCommand::isChar() const { return is_char; }

