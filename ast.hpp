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

    Scope* getScope() const;
    void setScope(Scope *sc);
    
    virtual AST* copyTree() const = 0;
	
    virtual void build_scope() = 0;    

    void define();
    void check();
    void gen();

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) = 0;
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) = 0;
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) = 0;

	virtual std::vector<AST*> getChildren() const = 0;
};

#endif
