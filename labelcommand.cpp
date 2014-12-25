#include "labelcommand.hpp"
#include "codeobject.hpp"
#include "commandvisitor.hpp"

LabelCommand::LabelCommand(Arg* label) : label(label)
{

}

void LabelCommand::gen(const Block&, CodeObject& code_obj) const
{
    code_obj.emit(label -> toString() + ":");
}

std::string LabelCommand::toString() const { return label -> toString() + ":"; }

bool LabelCommand::isExpr() const { return false; } 
const Type* LabelCommand::type() const { return nullptr; }

void LabelCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
