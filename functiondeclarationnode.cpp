#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, string> >& params, const vector<AST*>& statements) : name(name), params(params), statements(statements)
{

}

void FunctionDeclarationNode::build_scope()
{
    definedSymbol = new FunctionSymbol(name, nullptr, scope);    
    for ( auto i : statements )
    {
	i->scope = static_cast<FunctionSymbol*>(definedSymbol);
	i->build_scope();
    }
}

void FunctionDeclarationNode::define()
{
    
}

void FunctionDeclarationNode::gen()
{
    
}
