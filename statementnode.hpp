#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>
#include <algorithm>

#include "ast.hpp"

using std::vector;

class StatementNode : public AST
{       
public:

    StatementNode(vector<AST*> statements);

    virtual ~StatementNode();

    virtual AST* copyTree() const;
    
    virtual void define(const TemplateInfo& template_info);
    virtual void check(const TemplateInfo& template_info);
    virtual void gen(const TemplateInfo& template_info);

	virtual vector<AST*> getChildren() const;
			
private:

    vector<AST*> statements;
    
};

#endif
