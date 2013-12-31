#ifndef _FUNCTIONDECLARATIONNODE_HPP_
#define _FUNCTIONDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "functionsymbol.hpp"

using std::vector;
using std::pair;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(string name, const vector< pair<string, string> >& params, const vector<AST*>& statements);

    virtual void define();
    virtual void gen();
    
private:

    string name;
    vector< pair<string, string> > params;
    vector< AST* > statements;
};

#endif
