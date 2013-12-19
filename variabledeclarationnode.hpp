#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(string name, string type_name);

    virtual void define();
    virtual void gen();

private:

    string name, type_name;
};

#endif
