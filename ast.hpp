#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>

#include "semanticerror.hpp"
#include "globalhelper.hpp"

class Scope;
class TemplateStructSymbol;
class ExprNode;

class AST
{
public:

    virtual ~AST();

    virtual Scope* getScope() const;
    virtual void setScope(Scope *sc);
    
    virtual void build_scope() = 0;    
    virtual void define() = 0;
    virtual void check() = 0;
    virtual void gen() = 0;

    virtual void template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr) = 0;
//    virtual void template_gen(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr) = 0;
    virtual bool isTemplated() const = 0;
};

#endif
