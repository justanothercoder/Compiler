#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num), code_obj() { }

VariableType NumberNode::getType() const { return VariableType(BuiltIns::int_struct, false, true); }

void NumberNode::check(const TemplateInfo&) { getScope()->get_valloc()->addLocal(getType().getSize()); }

AST* NumberNode::copyTree() const { return new NumberNode(num); }

CodeObject& NumberNode::gen(const TemplateInfo&)
{
    code_obj.emit("mov qword [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "], " + num);
	code_obj.emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "]");
	
	return code_obj;
}
	
vector<AST*> NumberNode::getChildren() const { return { }; }
	
string NumberNode::getNum() const { return num; }
