#ifndef _UNARYNODE_HPP_
#define _UNARYNODE_HPP_

#include "exprnode.hpp"
#include "callinfo.hpp"
#include "functionsymbol.hpp"
#include "callhelper.hpp"
#include "codeobject.hpp"

enum class UnaryOp { PLUS, MINUS, NOT };

class UnaryNode : public ExprNode
{
public:
	
	UnaryNode(ExprNode *exp, UnaryOp op_type);

	virtual ~UnaryNode();
	virtual AST* copyTree() const;
	 virtual VariableType getType() const;

	string getOperatorName();
	string getCodeOperatorName();
    
	virtual void check(const TemplateInfo& template_info);
    virtual CodeObject& gen(const TemplateInfo& template_info);

	virtual vector<AST*> getChildren() const;

private:

	ExprNode *exp;
	UnaryOp op_type;
	CallInfo call_info;
	CodeObject code_obj;
};

#endif
