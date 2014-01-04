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
    Symbol *type = scope->resolve(type_info.type_name);

    if ( type == nullptr || dynamic_cast<Type*>(type) == nullptr )
	throw SemanticError("No such type " + type->getName());
    
    static_cast<VariableSymbol*>(definedSymbol)->setType(dynamic_cast<Type*>(type));

    scope->define(definedSymbol);
}

void VariableDeclarationNode::check()
{
    
}

void VariableDeclarationNode::gen()
{
    CodeGen::emit("sub rsp, " + std::to_string(static_cast<VariableSymbol*>(definedSymbol)->getType()->getSize()));
}
