#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>

#include "ast.hpp"

using std::vector;

class StatementNode : public AST
{       
public:

    StatementNode(const vector<AST*>& statements);

    virtual ~StatementNode();

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual void template_check();
    
private:

    vector<AST*> statements;
    
};

#endif
