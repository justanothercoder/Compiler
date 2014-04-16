#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num), code_obj() { }

void NumberNode::check(const TemplateInfo&) { getScope()->get_valloc()->addLocal(this, getType().getSize()); }

CodeObject& NumberNode::gen(const TemplateInfo&)
{
//    code_obj.emit("mov qword [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "], " + num);
//	code_obj.emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "]");
    code_obj.emit("mov qword [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(this)) + "], " + num);
	code_obj.emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(this)) + "]");
	
	return code_obj;
}
	
vector<AST*> NumberNode::getChildren() const { return { }; }
AST* NumberNode::copyTree() const { return new NumberNode(num); }
	
string NumberNode::getNum() const { return num; }

VariableType NumberNode::getType() const { return VariableType(BuiltIns::int_struct, false, true); }
bool NumberNode::isLeftValue() const { return false; }
