#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, const vector<AST*>& statements) : name(name), params(params), return_type_info(return_type_info), statements(statements)
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
    string return_type_name = return_type_info.type_name;
    
    Symbol *_return_type = scope->resolve(return_type_name);

    if ( _return_type == nullptr )
	throw SemanticError("No such symbol " + return_type_name);

    Type *return_type = dynamic_cast<Type*>(_return_type);
    
    if ( return_type == nullptr )
	throw SemanticError(return_type_name + " is not a type");

    vector<Type*> params_types;
    
    for ( auto i : params )
    {
	string param_type_name = i.second.type_name;
	
	Symbol *_param_type = scope->resolve(param_type_name);

	if ( _param_type == nullptr )
	    throw SemanticError("No such symbol " + param_type_name);

	Type *param_type = dynamic_cast<Type*>(_param_type);
	
	if ( param_type == nullptr )
	    throw SemanticError(param_type_name + " is not a type");

	params_types.push_back(param_type);

	static_cast<FunctionSymbol*>(definedSymbol)->define(new VariableSymbol(i.first, param_type));
    }

    FunctionType *function_type = new FunctionType(return_type, params_types);

    static_cast<FunctionSymbol*>(definedSymbol)->setType(function_type);

    scope->define(definedSymbol);
    
    for ( auto i : statements )
	i->define();	
}

void FunctionDeclarationNode::check()
{
    static_cast<FunctionSymbol*>(definedSymbol)->recalc_scope_address();
    
    for ( auto i : statements )
	i->check();
}

void FunctionDeclarationNode::gen()
{
    string typed_name = static_cast<FunctionSymbol*>(definedSymbol)->getTypedName();

    string scope_name = static_cast<FunctionSymbol*>(definedSymbol)->getEnclosingScope()->getScopeName() + "_";
    
    CodeGen::emit("jmp @" + scope_name + typed_name);
    CodeGen::emit(scope_name + typed_name + ":");
    CodeGen::emit("push rbp");
    CodeGen::emit("mov rbp, rsp");

    for ( auto i : statements )
	i->gen();
    
    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
    CodeGen::emit("@" + scope_name + typed_name + ":");
}
