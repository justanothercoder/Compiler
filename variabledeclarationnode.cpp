#include "variabledeclarationnode.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, string type_name) : name(name), type_name(type_name)
{
    definedSymbol = new VariableSymbol(name, nullptr);
}

void VariableDeclarationNode::build_scope()
{
    
}

void VariableDeclarationNode::define()
{
    Symbol *type = scope->resolve(type_name);

    if ( type == nullptr || dynamic_cast<Type*>(type) == nullptr )
	throw;
    
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
