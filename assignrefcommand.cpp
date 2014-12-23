#include "assignrefcommand.hpp"
#include "codeobject.hpp"
#include "type.hpp"
#include "builtins.hpp"
#include "commandvisitor.hpp"
#include "variablearg.hpp"
#include "temporaryarg.hpp"
#include "block.hpp"

AssignRefCommand::AssignRefCommand(Arg* lhs, Arg* rhs) : lhs(lhs), rhs(rhs)
{

}

void AssignRefCommand::gen(const Block& block, CodeObject& code_obj) const
{
    rhs -> gen(block, code_obj);
    code_obj.emit("mov rbx, rax");

    if ( auto _lhs = dynamic_cast<VariableArg*>(lhs) )
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(_lhs -> var)) + "]");
    }
    else if ( auto _lhs = dynamic_cast<TemporaryArg*>(lhs) )
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(_lhs -> command)) + "]");
    }
    else throw "";
        
    code_obj.emit("mov [rax], rbx");
}

std::string AssignRefCommand::toString() const { return lhs -> toString() + " = "  + rhs -> toString(); } 
bool AssignRefCommand::isExpr() const { return false; } 
const Type* AssignRefCommand::type() const { return nullptr; } 
void AssignRefCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
