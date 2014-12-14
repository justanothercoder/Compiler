#include "labelcommand.hpp"
#include "codeobject.hpp"

LabelCommand::LabelCommand(Arg* label) : label(label)
{

}

void LabelCommand::gen(const Block&, CodeObject& code_obj) const
{
    code_obj.emit(label -> toString() + ":");
}

std::string LabelCommand::toString() const
{
    return label -> toString();
}

bool LabelCommand::isExpr() const
{
    return false;
}
