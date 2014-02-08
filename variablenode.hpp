#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
#include "structsymbol.hpp"
#include "globalhelper.hpp"
#include "codegen.hpp"
#include "templatestructsymbol.hpp"
#include "classvariablesymbol.hpp"

class VariableNode : public ExprNode
{
public:

    VariableNode(string name);

    virtual AST* copyTree() const;

    virtual Type *getType() const;
    virtual bool isLeftValue() const;    

    virtual void build_scope();
    virtual void check();
    virtual void gen();

    virtual void template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    
private:

    string name;
    
    VariableSymbol *variable;
};

#endif
