#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, string> >& params, string return_type_name, const vector<AST*>& statements) : name(name), params(params), return_type_name(return_type_name), statements(statements)
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
    Symbol *return_type = scope->resolve(return_type_name);

    if ( return_type == nullptr || dynamic_cast<Type*>(return_type) == nullptr )
	throw;

    vector<Type*> params_types;
    
    for ( auto i : params )
    {
	Symbol *param_type = scope->resolve(i.second);

	if ( return_type == nullptr || dynamic_cast<Type*>(return_type) == nullptr )
	    throw;

	params_types.push_back(dynamic_cast<Type*>(param_type));

	static_cast<FunctionSymbol*>(definedSymbol)->define(new VariableSymbol(i.first, dynamic_cast<Type*>(param_type)));
    }

    FunctionType *function_type = new FunctionType(dynamic_cast<Type*>(return_type), params_types);

    static_cast<FunctionSymbol*>(definedSymbol)->setType(function_type);

    scope->define(definedSymbol);    
    
    for ( auto i : statements )
	i->define();	
}

void FunctionDeclarationNode::gen()
{
    for ( auto i : statements )
	i->gen();
}
