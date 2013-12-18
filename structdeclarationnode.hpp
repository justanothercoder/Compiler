#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"

using std::vector;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(string name, const vector<AST*>& inner);

    virtual void define();
    virtual void gen();

private:

    string name;
    vector<AST*> inner;
};

#endif
