#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num), code_obj() { }

void NumberNode::check() 
{ 
	scope -> getTempAlloc().add(getType().getSize());
}

CodeObject& NumberNode::gen()
{
	std::string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";

	code_obj.emit("mov qword " + addr + ", " + num);
	code_obj.emit("lea rax, " + addr);
	
	scope -> getTempAlloc().claim(getType().getSize());

	return code_obj;
}
	
AST* NumberNode::copyTree() const { return new NumberNode(num); }
	
string NumberNode::getNum() const { return num; }

VariableType NumberNode::getType() const { return VariableType(BuiltIns::int_struct, false, true); }
bool NumberNode::isLeftValue() const { return false; }

int NumberNode::neededSpaceForTemporaries()
{
	return GlobalConfig::int_size;
}

void NumberNode::freeTempSpace()
{
	scope -> getTempAlloc().free();
}
