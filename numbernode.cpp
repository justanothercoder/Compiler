#include "numbernode.hpp"
#include "globalconfig.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"

NumberNode::NumberNode(std::string num) : num(num)
{

}

AST* NumberNode::copyTree() const 
{
   	return new NumberNode(num); 
}
	
const Type* NumberNode::getType() const
{
   	return TypeFactory::getConst(BuiltIns::int_type); 
}

bool NumberNode::isLeftValue() const 
{
   	return false; 
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

void NumberNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
