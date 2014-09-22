#include "ast.hpp"

AST::~AST() 
{

}
 
void AST::build_scope()
{
	std::vector<AST*> children = getChildren();

	for ( auto child : children )
	{
		child -> scope = scope;
		child -> build_scope();
	}
}
	
std::vector<AST*> AST::getChildren() const 
{
   	return { }; 
}
