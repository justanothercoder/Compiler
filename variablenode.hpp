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

    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;

    bool isTemplateParam() const;
    
private:

    string name;
    
    VariableSymbol *variable;

    TemplateStructSymbol *template_sym;
    vector<ExprNode*> template_expr;
};

#endif
