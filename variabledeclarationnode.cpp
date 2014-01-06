#include "variabledeclarationnode.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, TypeInfo type_info, bool is_field) : name(name), type_info(type_info), is_field(is_field)
{
    definedSymbol = new VariableSymbol(name, nullptr);
}

void VariableDeclarationNode::build_scope()
{
    
}

void VariableDeclarationNode::define()
{
    Type *var_type = TypeHelper::fromTypeInfo(type_info, scope);    
    static_cast<VariableSymbol*>(definedSymbol)->setType(var_type);
    scope->define(definedSymbol);
}

void VariableDeclarationNode::check()
{
    
}

void VariableDeclarationNode::gen()
{
    if ( !is_field )
	CodeGen::emit("sub rsp, " + std::to_string(static_cast<VariableSymbol*>(definedSymbol)->getType()->getSize()));
}
