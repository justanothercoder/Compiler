#include "variabledeclarationnode.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, string type_name) : name(name), type_name(type_name)
{
    definedSymbol = new VariableSymbol("", nullptr);
}

void VariableDeclarationNode::define()
{
    Symbol *type = scope->resolve(type_name);
    
    *definedSymbol = VariableSymbol(name, (Type*)type);

    scope->define(definedSymbol);
}

void VariableDeclarationNode::gen()
{
    CodeGen::emit("rsp, " + std::to_string(definedSymbol->getType()->getSize()));
}
