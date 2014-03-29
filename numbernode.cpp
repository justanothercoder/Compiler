#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num) { }

VariableType NumberNode::getType() const { return VariableType(BuiltIns::int_struct, false, true); }

void NumberNode::check(const TemplateInfo&) { getScope()->get_valloc()->addLocal(getType().getSize()); }

AST* NumberNode::copyTree() const { return new NumberNode(num); }

void NumberNode::gen(const TemplateInfo&)
{
    CodeGen::emit("mov qword [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "], " + num);
	CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "]");
}
	
vector<AST*> NumberNode::getChildren() const { return { }; }
	
string NumberNode::getNum() const { return num; }
