#include "gotocommand.hpp"
#include "codeobject.hpp"

GotoCommand::GotoCommand(Arg* label) : label(label)
{

}

void GotoCommand::gen(const Block&, CodeObject& code_obj) const
{
    code_obj.emit("jmp " + label -> toString());
}

std::string GotoCommand::toString() const
{
    return "goto " + label -> toString();
}

bool GotoCommand::isExpr() const
{
    return false;
}
