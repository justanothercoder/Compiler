#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>
#include <algorithm>

#include "ast.hpp"

using std::vector;

class StatementNode : public AST
{       
public:

    StatementNode(const vector<AST*>& statements);

    virtual ~StatementNode();

    virtual AST* copyTree() const;
    
    virtual void build_scope();

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;
			
private:

    vector<AST*> statements;
    
};

#endif
