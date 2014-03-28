#include "unarynode.hpp"

UnaryNode::UnaryNode(ExprNode *exp, UnaryOp op_type) : exp(exp), op_type(op_type) { }

UnaryNode::~UnaryNode() { delete exp; } 

AST* UnaryNode::copyTree() const { return new UnaryNode(static_cast<ExprNode*>(exp->copyTree()), op_type); }

VariableType UnaryNode::getType() const { return call_info.callee->getTypeInfo().return_type; }

string UnaryNode::getOperatorName()
{
	switch ( op_type )
	{
		case UnaryOp::PLUS : return "operator+";
		case UnaryOp::MINUS: return "operator-";
		case UnaryOp::NOT  : return "operator!";
	}
}

string UnaryNode::getCodeOperatorName()
{
	switch ( op_type )
	{
		case UnaryOp::PLUS : return "operatorplus";
		case UnaryOp::MINUS: return "operatorminus";
		case UnaryOp::NOT  : return "operatornot";
	}
}

void UnaryNode::check(const TemplateInfo& template_info)
{
	exp->check(template_info);
	call_info = CallHelper::callCheck(getOperatorName(), static_cast<StructSymbol*>(exp->getType().type), { }, template_info);
}

void UnaryNode::gen(const TemplateInfo& template_info)
{
	CodeGen::genCallCode(call_info, { }, template_info, [&]() { exp->gen(template_info); });	
}

std::vector<AST*> UnaryNode::getChildren() const { return {exp}; }
