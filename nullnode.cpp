#include "nullnode.hpp"
#include "scope.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"

NullNode::NullNode() 
{

}
	
AST* NullNode::copyTree() const 
{
	return new NullNode();
}
	
std::string NullNode::toString() const 
{
	return "null";
}
    
const Type* NullNode::getType() const 
{
	return TypeFactory::getPointer(BuiltIns::void_type);
}

bool NullNode::isLeftValue() const
{
	return false;
}

void NullNode::freeTempSpace() 
{
	
}

bool NullNode::isCompileTimeExpr() const 
{
	return false;
}

boost::optional<int> NullNode::getCompileTimeValue() const 
{
	return boost::none;
}

void NullNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
