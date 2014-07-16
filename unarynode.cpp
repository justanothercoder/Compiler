#include "unarynode.hpp"

UnaryNode::UnaryNode(ExprNode *exp, UnaryOp op_type) : exp(exp), op_type(op_type) { }
UnaryNode::~UnaryNode() { delete exp; } 

void UnaryNode::check()
{
	exp -> check();
	call_info = CallHelper::callCheck(getOperatorName(), static_cast<StructSymbol*>(exp -> getType().type), { });
	
	scope -> getTempAlloc().add(getType().getSize());
}

CodeObject& UnaryNode::gen()
{
	string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(getType().getSize());

	code_obj.emit("lea rax, " + addr);
	code_obj.emit("push rax");

	code_obj.genCallCode(call_info, { }, exp -> gen(), exp -> getType().is_ref);
	
	code_obj.emit("pop rax");

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
AST* UnaryNode::copyTree() const { return new UnaryNode(static_cast<ExprNode*>(exp -> copyTree()), op_type); }

VariableType UnaryNode::getType() const { return call_info.callee -> return_type; }
bool UnaryNode::isLeftValue() const { return false; }

void UnaryNode::freeTempSpace() 
{

}

bool UnaryNode::isCompileTimeExpr() const
{
	return exp -> isCompileTimeExpr() && call_info.callee -> is_constexpr;
}

optional<int> UnaryNode::getCompileTimeValue() const
{
	return optional<int>::empty();
}
