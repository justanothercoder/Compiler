#ifndef _NUMBERNODE_HPP_
#define _NUMBERNODE_HPP_

#include "exprnode.hpp"
#include "globalconfig.hpp"
#include "builtins.hpp"

class NumberNode : public ExprNode
{
public:

    NumberNode(string num);

    virtual AST* copyTree() const;

    virtual void build_scope();
    
    virtual Type* getType() const;
    virtual bool isLeftValue() const;

    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;

	string getNum() const;

private:

    string num;
    
};
    
#endif
