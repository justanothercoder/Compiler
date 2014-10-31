#include "variablenode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"

VariableNode::VariableNode(std::string name) : name(name), variable(nullptr), template_num(nullptr) 
{

}

bool VariableNode::isTemplateParam() const 
{ 
	const auto& template_info = scope -> getTemplateInfo();
	return template_info.sym != nullptr && template_info.sym -> isIn(name); 
}
	
AST* VariableNode::copyTree() const { return new VariableNode(name); }

const Type* VariableNode::getType() const
{
	if ( isTemplateParam() )
	{
		const auto& template_info = scope -> getTemplateInfo();

		auto replace = template_info.getReplacement(name);

		return BuiltIns::global_scope -> resolveType("int");
	}

	return variable -> getType();
}

bool VariableNode::isLeftValue() const 
{
   	return true; 
}

bool VariableNode::isCompileTimeExpr() const
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym != nullptr && template_info.sym -> isIn(name) )
		return true;
	else
		return false;
}

boost::optional<int> VariableNode::getCompileTimeValue() const
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym != nullptr && template_info.sym -> isIn(name) )
		return boost::get<int>(*template_info.getReplacement(name));
	else
		return boost::none;
}

std::string VariableNode::toString() const
{
	return name;
}
	
void VariableNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
