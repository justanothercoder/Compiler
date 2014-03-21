#include "stringnode.hpp"

StringNode::StringNode(string str) : str(str) { }

VariableType StringNode::getType() const { return VariableType(BuiltIns::string_struct, false, true); }

void StringNode::check(const TemplateInfo&) { }

AST* StringNode::copyTree() const { return new StringNode(str); }

void StringNode::gen(const TemplateInfo&)
{
	string str_label = StringNode::getNewLabel();

	CodeGen::emit("section .data");
	CodeGen::emit(str_label + ": db \"" + str + "\",0");
	CodeGen::emit("section .text");
	CodeGen::emit("lea rax, [" + str_label + "]");
}
	
vector<AST*> StringNode::getChildren() const { return { }; }
	
string StringNode::getStr() const { return str; }

string StringNode::getNewLabel()
{
	static int label_num = 0;
	return "@string_label" + std::to_string(++label_num);
}
