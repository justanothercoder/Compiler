#include "stringnode.hpp"
#include "typehelper.hpp"
#include "typefactory.hpp"

StringNode::StringNode(std::string str) : str(str) 
{

}

void StringNode::check() 
{

}

CodeObject& StringNode::gen()
{
	string str_label = StringNode::getNewLabel();

	code_obj.emit("section .data");

	string res = "0";

	for ( int i = str.length() - 1; i >= 0; --i )
	{
		if ( i >= 1 && str[i - 1] == '\\' && str[i] == 'n' )
		{
			res += ", 10";
			--i;
		}
		else
			res += ", " + std::to_string(static_cast<int>(str[i]));
	}

	code_obj.emit("@" + str_label + ": db " + res);
	code_obj.emit(str_label + " equ $ - 1");

	code_obj.emit("section .text");
	code_obj.emit("lea rax, [" + str_label + "]");

	return code_obj;
}
	
AST* StringNode::copyTree() const 
{
   	return new StringNode(str); 
}
	
std::string StringNode::getStr() const 
{
   	return str; 
}

std::string StringNode::getNewLabel()
{
	static int label_num = 0;
	return "@string_label" + std::to_string(++label_num);
}

const Type* StringNode::getType() const 
{
	static const Type *type = TypeHelper::resolveType("string", BuiltIns::global_scope);

	return TypeFactory::getConst(type);
}

bool StringNode::isLeftValue() const 
{
   	return false; 
}

void StringNode::freeTempSpace()
{

}

bool StringNode::isCompileTimeExpr() const
{
	return false;
}

boost::optional<int> StringNode::getCompileTimeValue() const
{
	return boost::none;
}
