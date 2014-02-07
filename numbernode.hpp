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
    virtual void check();    
    virtual void gen();
    
    virtual Type* getType() const;
    virtual bool isLeftValue() const;

    virtual void template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual bool isTemplated() const;
    
private:

    string num;
    
};
    
#endif
