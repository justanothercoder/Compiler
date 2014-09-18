#include "numbernode.hpp"
#include "globalconfig.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"
#include "globalhelper.hpp"

NumberNode::NumberNode(std::string num) : num(num)
{
	int num_value = std::stoi(num);

	auto& table = GlobalHelper::const_num_id;

	if ( table.find(num_value) == std::end(table) )
		GlobalHelper::addConst(num_value);
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
	
std::string NumberNode::toString() const
{
	return num;
}

void NumberNode::accept(ASTVisitor *visitor)
{
	visitor -> visit(this);
}

int NumberNode::getNumId() const
{
	return GlobalHelper::const_num_id[std::stoi(num)];
}
