#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, const vector<AST*>& statements, bool is_method) : name(name), params(params), return_type_info(return_type_info), statements(statements), is_method(is_method)
{

}

FunctionDeclarationNode::~FunctionDeclarationNode() { delete definedSymbol; }   

void FunctionDeclarationNode::build_scope()
{
    definedSymbol = new FunctionSymbol(name, FunctionTypeInfo(nullptr, { }, is_method), scope, false, is_method);    
    for ( auto i : statements )
    {
	i->scope = definedSymbol;
	i->build_scope();
    }
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void FunctionDeclarationNode::define()
{
    Type *return_type = TypeHelper::fromTypeInfo(return_type_info, scope);
    
    vector<Type*> params_types;
    
    if ( is_method )
    {
	Type *_this_type = TypeHelper::getReferenceType(static_cast<StructSymbol*>(scope));
	params_types.push_back(_this_type);

	definedSymbol->define(new VariableSymbol("this", _this_type, VariableSymbolType::PARAM));
    }
    
    for ( auto i : params )
    {
	Type *param_type = TypeHelper::fromTypeInfo(i.second, scope);	
	params_types.push_back(param_type);

	definedSymbol->define(new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM));
    }

    FunctionTypeInfo function_type_info = FunctionTypeInfo(return_type, params_types, is_method);

    definedSymbol->setTypeInfo(function_type_info);

    scope->define(definedSymbol);
    
    for ( auto i : statements )
	i->define();	
}

void FunctionDeclarationNode::check()
{
    definedSymbol->recalc_scope_address();
    
    for ( auto i : statements )
	i->check();
}

void FunctionDeclarationNode::gen()
{
    string scoped_typed_name = definedSymbol->getScopedTypedName();
    
    CodeGen::emit("jmp _~" + scoped_typed_name);
    CodeGen::emit(scoped_typed_name + ":");
    CodeGen::emit("push rbp");
    CodeGen::emit("mov rbp, rsp");

    for ( auto i : statements )
	i->gen();
    
    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
    CodeGen::emit("_~" + scoped_typed_name + ":");
}
