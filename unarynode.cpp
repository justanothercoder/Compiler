#include "unarynode.hpp"

UnaryNode::UnaryNode(ExprNode *exp, UnaryOp op_type) : exp(exp), op_type(op_type) { }
UnaryNode::~UnaryNode() { delete exp; } 

void UnaryNode::check()
{
	exp->check();
	call_info = CallHelper::callCheck(getOperatorName(), static_cast<StructSymbol*>(exp->getType().type), { });
	
	scope->get_valloc()->addReturnValueSpace(getType().getSize());
}

CodeObject& UnaryNode::gen()
{
	code_obj.genCallCode(call_info, { }, exp->gen(), exp->getType().is_ref);
	return code_obj;
}

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

std::vector<AST*> UnaryNode::getChildren() const { return {exp}; }
AST* UnaryNode::copyTree() const { return new UnaryNode(static_cast<ExprNode*>(exp->copyTree()), op_type); }

VariableType UnaryNode::getType() const { return call_info.callee->function_type_info.return_type; }
bool UnaryNode::isLeftValue() const { return false; }

int UnaryNode::neededSpaceForTemporaries()
{
	return exp -> neededSpaceForTemporaries();
}
