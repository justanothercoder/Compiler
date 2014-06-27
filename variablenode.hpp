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

    virtual void check();
    virtual CodeObject& gen();

    virtual AST* copyTree() const;

    bool isTemplateParam() const;
    
    virtual VariableType getType() const;
	virtual bool isLeftValue() const;

	virtual int neededSpaceForTemporaries();

private:

    string name;
    
    VariableSymbol *variable;

	TemplateInfo _template_info;
	CodeObject *code_obj;
};

#endif
