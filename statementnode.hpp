#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>
#include <algorithm>

#include "ast.hpp"
#include "codeobject.hpp"

using std::vector;

class StatementNode : public AST
{       
public:

    StatementNode(vector<AST*> statements);

    virtual ~StatementNode();

    virtual AST* copyTree() const;
    
    virtual void define();
    virtual void check();
    virtual CodeObject& gen();

	virtual vector<AST*> getChildren() const;
			
private:

    vector<AST*> statements;
	CodeObject code_obj;    
};

#endif
