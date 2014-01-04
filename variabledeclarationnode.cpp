#include "variabledeclarationnode.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, TypeInfo type_info) : name(name), type_info(type_info)
{
    definedSymbol = new VariableSymbol(name, nullptr);
}

void VariableDeclarationNode::build_scope()
{
    
}

void VariableDeclarationNode::define()
{
    string type_name = type_info.type_name;
    
    Symbol *type = scope->resolve(type_name);

    if ( type == nullptr )
	throw SemanticError("No such symbol " + type_name);

    Type *var_type = dynamic_cast<Type*>(type);

    if ( var_type == nullptr )
	throw SemanticError(type_name + " is not a type");

    if ( type_info.is_ref )
	var_type = new ReferenceType(var_type);
    
    static_cast<VariableSymbol*>(definedSymbol)->setType(var_type);

    scope->define(definedSymbol);
}

void VariableDeclarationNode::check()
{
    
}

void VariableDeclarationNode::gen()
{
    CodeGen::emit("sub rsp, " + std::to_string(static_cast<VariableSymbol*>(definedSymbol)->getType()->getSize()));
}
