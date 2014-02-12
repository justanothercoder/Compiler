#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "referencetype.hpp"
#include "typehelper.hpp"
#include "globalhelper.hpp"
#include "codegen.hpp"
#include "templatestructsymbol.hpp"

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, string member_name);   

	virtual ~DotNode();
    
    virtual AST* copyTree() const;

    virtual void build_scope();

    virtual Type* getType() const;
    virtual bool isLeftValue() const;

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);    
    
private:

    ExprNode *base;

    string member_name;

    StructSymbol *base_type;
    VariableSymbol *member;
};

#endif
