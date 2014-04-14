#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
#include "structsymbol.hpp"
#include "globalhelper.hpp"
#include "codeobject.hpp"
#include "templatestructsymbol.hpp"
#include "classvariablesymbol.hpp"
#include "templateinfo.hpp"

class VariableNode : public ExprNode
{
public:

    VariableNode(string name);
	~VariableNode();

    virtual void check(const TemplateInfo& template_info);
    virtual CodeObject& gen(const TemplateInfo& template_info);

	virtual vector<AST*> getChildren() const;
    virtual AST* copyTree() const;

    bool isTemplateParam() const;
    
    virtual VariableType getType() const;
	virtual bool isLeftValue() const;

private:

    string name;
    
    VariableSymbol *variable;

	TemplateInfo template_info;
	CodeObject *code_obj;
};

#endif
