#include "stringnode.hpp"

StringNode::StringNode(string str) : str(str) { }

VariableType StringNode::getType() const 
{
	static Type *type = TypeHelper::resolveType("string", BuiltIns::global_scope);

	return VariableType(type, false, true); 
}

void StringNode::check(const TemplateInfo&) { }

AST* StringNode::copyTree() const { return new StringNode(str); }

void StringNode::gen(const TemplateInfo&)
{
	string str_label = StringNode::getNewLabel();

	CodeGen::emit("section .data");

	string s = str;
	reverse(std::begin(s), std::end(s));

	CodeGen::emit("@" + str_label + ": dq 0,\"" + s.substr(0, s.length() - 1) + '"');
	CodeGen::emit(str_label + ": dq \"" + s.back() + "\"");
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
