#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "globalhelper.hpp"
#include "templatestructsymbol.hpp"
#include "classvariablesymbol.hpp"
#include "templateinfo.hpp"
#include "codeobject.hpp"

class VariableNode : public ExprNode
{
public:

    VariableNode(string name);

    void check() override;
    CodeObject& gen() override;

    AST* copyTree() const override;

    bool isTemplateParam() const;
    
    VariableType getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

private:

    string name;
    
    VariableSymbol *variable;

	TemplateInfo _template_info;
	CodeObject code_obj;
};

#endif
