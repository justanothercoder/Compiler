#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "referencetype.hpp"
#include "typehelper.hpp"
#include "globalhelper.hpp"
#include "codegen.hpp"
#include "templatehelper.hpp"
#include "templatestructsymbol.hpp"

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, string member_name);   
    ~DotNode();
    
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

    ExprNode *base;

    string member_name;

    StructSymbol *base_type;
    Symbol *member;
};

#endif
