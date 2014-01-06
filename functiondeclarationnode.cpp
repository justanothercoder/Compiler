#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, const vector<AST*>& statements, bool is_method) : name(name), params(params), return_type_info(return_type_info), statements(statements), is_method(is_method)
{

}

void FunctionDeclarationNode::build_scope()
{
    definedSymbol = new FunctionSymbol(name, FunctionTypeInfo(nullptr, { }), scope, false, is_method);    
    for ( auto i : statements )
    {
	i->scope = static_cast<FunctionSymbol*>(definedSymbol);
	i->build_scope();
    }
}

void FunctionDeclarationNode::define()
{
    Type *return_type = TypeHelper::fromTypeInfo(return_type_info, scope);
    
    vector<Type*> params_types;
    
    if ( is_method )
	static_cast<FunctionSymbol*>(definedSymbol)->define(new VariableSymbol("this", TypeHelper::getReferenceType(static_cast<StructSymbol*>(scope)), VariableSymbolType::PARAM));
    
    for ( auto i : params )
    {
	Type *param_type = TypeHelper::fromTypeInfo(i.second, scope);	
	params_types.push_back(param_type);

	static_cast<FunctionSymbol*>(definedSymbol)->define(new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM));
    }

    FunctionTypeInfo function_type_info = FunctionTypeInfo(return_type, params_types);

    static_cast<FunctionSymbol*>(definedSymbol)->setTypeInfo(function_type_info);

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
