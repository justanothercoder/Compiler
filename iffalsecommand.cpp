#include "iffalsecommand.hpp"
#include "codeobject.hpp"
#include "commandvisitor.hpp"

IfFalseCommand::IfFalseCommand(Argument expr, Argument label_false) : expr_(expr), label_false(label_false) { }

void IfFalseCommand::gen(const Block& block, CodeObject& code_obj) const
{
    expr_ -> gen(block, code_obj);
    code_obj.emit("cmp qword [rax], qword 0");
    code_obj.emit("jz " + label_false -> toString());
}

std::string IfFalseCommand::toString() const { return "ifFalse " + expr_ -> toString() + " goto " + label_false -> toString(); }

bool IfFalseCommand::isExpr() const { return false; }
const Type* IfFalseCommand::type() const { return nullptr; }

void IfFalseCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
    
Arg* IfFalseCommand::expr() { return expr_.get(); }
Arg* IfFalseCommand::branch() { return label_false.get(); }

