#include "functiondeclarationnode.hpp"

#include "typefactory.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(std::string name
		                                       , std::vector< pair<std::string, TypeInfo> > params
											   , TypeInfo return_type_info
											   , AST *statements
											   , FunctionTraits traits
											   , bool is_unsafe) : name(name)
																 , params(params)
																 , return_type_info(return_type_info)
																 , statements(statements)
																 , traits(traits)
																 , definedSymbol(nullptr) 
																 , is_unsafe(is_unsafe)
{

}

void FunctionDeclarationNode::build_scope()
{
	definedSymbol = new FunctionSymbol(traits.is_constructor ? static_cast<StructSymbol*>(scope) -> getName() : name, nullptr, { }, scope, traits);
	definedSymbol -> is_unsafe = is_unsafe;

	statements -> scope = definedSymbol;
	statements -> build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

AST* FunctionDeclarationNode::copyTree() const 
{ 
	return new FunctionDeclarationNode(name, params, return_type_info, statements -> copyTree(), traits); 
}

std::vector<AST*> FunctionDeclarationNode::getChildren() const 
{ 
	return {statements}; 
}
	
std::string FunctionDeclarationNode::toString() const
{
	std::string res = return_type_info.toString() + " " + name + "(";
	
	if ( !params.empty() )
	{
		auto it = std::begin(params);

		res += it -> second.toString() + " " + it -> first;

		for ( ++it; it != std::end(params); ++it )
			res += ", " + it -> second.toString() + " " + it -> first;
	}

	res += ")";
	res += statements -> toString();

	return res;
}

void FunctionDeclarationNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
