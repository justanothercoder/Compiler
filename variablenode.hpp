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
	~VariableNode() override;

    void check() override;
    CodeObject& gen() override;

    AST* copyTree() const override;

    bool isTemplateParam() const;
    
    VariableType getType() const override;
	bool isLeftValue() const override;

	int neededSpaceForTemporaries() override;

	void freeTempSpace() override;

private:

    string name;
    
    VariableSymbol *variable;

	TemplateInfo _template_info;
	CodeObject *code_obj;
};

#endif
