#include "variabledeclarationnode.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, string type_name) : name(name), type_name(type_name)
{
    definedSymbol = new VariableSymbol("", nullptr);
}

void VariableDeclarationNode::define()
{
    Symbol *type = scope->resolve(type_name);

    if ( type == nullptr || dynamic_cast<Type*>(type) == nullptr )
	throw;
    
    definedSymbol->setName(name);
    static_cast<VariableSymbol*>(definedSymbol)->setType(dynamic_cast<Type*>(type));

    scope->define(definedSymbol);
}

void VariableDeclarationNode::gen()
{
    CodeGen::emit("sub rsp, " + std::to_string(static_cast<VariableSymbol*>(definedSymbol)->getType()->getSize()));
}
