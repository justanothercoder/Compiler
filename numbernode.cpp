#include "numbernode.hpp"
#include "globalconfig.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"
#include "globalhelper.hpp"

NumberNode::NumberNode(std::string num) : num(num)
{

}

void NumberNode::check() 
{ 
	scope -> getTempAlloc().add(getType() -> getSize());
}

CodeObject& NumberNode::gen()
{
	std::string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";

	code_obj.emit("mov qword " + addr + ", " + num);
	code_obj.emit("lea rax, " + addr);
	
	scope -> getTempAlloc().claim(getType() -> getSize());

	return code_obj;
}
	
AST* NumberNode::copyTree() const 
{
   	return new NumberNode(num); 
}
	
std::string NumberNode::getNum() const
{
   	return num; 
}

const Type* NumberNode::getType() const
{
   	return TypeFactory::getConst(BuiltIns::int_type); 
}

bool NumberNode::isLeftValue() const 
{
   	return false; 
}

void NumberNode::freeTempSpace()
{
	scope -> getTempAlloc().free();
}

bool NumberNode::isCompileTimeExpr() const
{
	return true;
}

boost::optional<int> NumberNode::getCompileTimeValue() const
{
	return boost::optional<int>(std::stoi(num));
}
