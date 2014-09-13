#include "nullnode.hpp"
#include "scope.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"
#include "globalhelper.hpp"

NullNode::NullNode() 
{

}
	
AST* NullNode::copyTree() const 
{
	return new NullNode();
}
	
void NullNode::check() 
{
	scope -> getTempAlloc().add(getType() -> getSize());
}

CodeObject& NullNode::gen() 
{
	auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";

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
