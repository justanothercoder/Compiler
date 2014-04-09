#include "stringnode.hpp"

StringNode::StringNode(string str) : str(str) { }

VariableType StringNode::getType() const 
{
	static Type *type = TypeHelper::resolveType("string", BuiltIns::global_scope);

	return VariableType(type, false, true); 
}

void StringNode::check(const TemplateInfo&) { getScope()->get_valloc()->addLocal(256 * 8); }

AST* StringNode::copyTree() const { return new StringNode(str); }

void StringNode::gen(const TemplateInfo&)
{
	string str_label = StringNode::getNewLabel();

	CodeGen::emit("section .data");

	string res = "0";

	for ( int i = str.length() - 1; i >= 1; --i )
		res += ", " + std::to_string(static_cast<int>(str[i]));

//	CodeGen::emit("@" + str_label + ": dq " + res);
//	CodeGen::emit(str_label + ": dq " + std::to_string(static_cast<int>(str[0])));
	CodeGen::emit("@" + str_label + ": db " + res);
	CodeGen::emit(str_label + ": db " + std::to_string(static_cast<int>(str[0])));
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
