#ifndef _FUNCTIONDECLARATIONNODE_HPP_
#define _FUNCTIONDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

using std::vector;
using std::pair;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(string name, const vector< pair<string, string> >& params, string return_type_name, const vector<AST*>& statements);

    virtual void build_scope();    
    virtual void define();
    virtual void check();
    virtual void gen();
    
private:

    string name;
    vector< pair<string, string> > params;
    string return_type_name;
    vector< AST* > statements;
};

#endif
