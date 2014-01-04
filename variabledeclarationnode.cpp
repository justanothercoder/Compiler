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
    Type *var_type = TypeHelper::fromTypeInfo(type_info, scope);    
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
