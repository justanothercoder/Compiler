#ifndef _DECLARATIONNODE_HPP_
#define _DECLARATIONNODE_HPP_

#include "ast.hpp"

class Symbol;

class DeclarationNode : public AST
{
public:

    virtual ~DeclarationNode();

    virtual void define() = 0;
    virtual void gen() = 0;

    virtual void process();
    
protected:

    Symbol *definedSymbol;
    int definitionTime;
};

#endif
