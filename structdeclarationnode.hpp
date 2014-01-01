#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "structsymbol.hpp"

using std::vector;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(string name, const vector<DeclarationNode*>& inner);

    virtual void build_scope();
    
    virtual void define();
    virtual void gen();

private:

    string name;
    vector<DeclarationNode*> inner;
};

#endif
