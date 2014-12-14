#include "labelarg.hpp"
#include "codeobject.hpp"

LabelArg::LabelArg(std::string label) : label(label)
{

}

void LabelArg::gen(const Block&, CodeObject& code_obj) const
{
    code_obj.emit(label + ":");
}

std::string LabelArg::toString() const
{
    return label + ":";
}
