#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num) { }

VariableType NumberNode::getType() const { return VariableType(BuiltIns::int_struct, false, true); }

void NumberNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }

AST* NumberNode::copyTree() const { return new NumberNode(num); }

void NumberNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    CodeGen::emit("mov qword [rsp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal() + getType().getSize()) + "], " + num);
	CodeGen::emit("lea rax, [rsp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal() + getType().getSize()) + "]");
}
	
vector<AST*> NumberNode::getChildren() const { return { }; }
	
string NumberNode::getNum() const { return num; }
