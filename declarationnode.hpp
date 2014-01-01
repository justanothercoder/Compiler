#ifndef _DECLARATIONNODE_HPP_
#define _DECLARATIONNODE_HPP_

#include "ast.hpp"
#include "codegen.hpp"

class Symbol;

class DeclarationNode : public AST
{
public:

    virtual ~DeclarationNode();

protected:

    Symbol *definedSymbol;
    int definitionTime;
};

#endif
