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
	
CodeObject& NullNode::gen() 
{
	auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";

	scope -> getTempAlloc().claim(getType() -> getSize());

	code_obj.emit("mov qword " + addr + ", 0");
	code_obj.emit("lea rax, " + addr);

	return code_obj;
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
