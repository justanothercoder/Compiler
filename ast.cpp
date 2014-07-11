#include "ast.hpp"

AST::~AST() { }
 
void AST::build_scope()
{
	std::vector<AST*> children = getChildren();

	for ( auto child : children )
	{
		child -> scope         = scope;
		child -> template_info = template_info;

		child -> build_scope();
	}
}
	
std::vector<AST*> AST::getChildren() const { return { }; } 

