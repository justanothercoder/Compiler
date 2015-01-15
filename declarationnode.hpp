#ifndef _DECLARATIONNODE_HPP_
#define _DECLARATIONNODE_HPP_

#include "ast.hpp"

class Symbol;

class DeclarationNode : public AST
{
public:

    ~DeclarationNode() override;
    virtual const Symbol* getDefinedSymbol() const = 0;
};

#endif
