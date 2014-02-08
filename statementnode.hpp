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
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual void template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
	
private:

    vector<AST*> statements;
    
};

#endif
