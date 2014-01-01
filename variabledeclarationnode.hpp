#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "variablesymbol.hpp"
#include "codegen.hpp"

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(string name, string type_name);

    virtual void build_scope();    
    virtual void define();
    virtual void check();
    virtual void gen();

private:

    string name, type_name;
};

#endif
