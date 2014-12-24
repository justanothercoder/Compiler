#include "ifcommand.hpp"

void IfCommand::gen(const Block& block, CodeObject& code_obj)
{
    expr -> gen(block, code_obj);
    code_obj.emit("cmp qword [rax], qword 0");
    code_obj.emit("jnz " + label -> toString());
}

std::string IfCommand::toString() const { return "if " + expr -> toString() + " goto " + label -> toString(); }
