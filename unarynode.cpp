#include "unarynode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "globalhelper.hpp"

UnaryNode::UnaryNode(ExprNode *exp, UnaryOp op_type) : exp(exp), op_type(op_type) 
{

}

UnaryNode::~UnaryNode()
{
   	delete exp; 
}	

void UnaryNode::check()
{
	exp -> check();
	call_info = CallHelper::callCheck(getOperatorName(), static_cast<const StructSymbol*>(exp -> getType() -> getSymbol()), { });
	
	scope -> getTempAlloc().add(getType() -> getSize());
}

CodeObject& UnaryNode::gen()
{
	auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(getType() -> getSize());

	code_obj.emit("lea rax, " + addr);
	code_obj.emit("push rax");

	code_obj.genCallCode(call_info, { }, exp -> gen(), exp -> getType() -> isReference());
	
	code_obj.emit("pop rax");

	return code_obj;
}

std::string UnaryNode::getOperatorName()
{
	switch ( op_type )
	{
		case UnaryOp::PLUS : return "operator+";
		case UnaryOp::MINUS: return "operator-";
		case UnaryOp::NOT  : return "operator!";
	}
}

std::string UnaryNode::getCodeOperatorName()
{
	switch ( op_type )
	{
		case UnaryOp::PLUS : return "operatorplus";
		case UnaryOp::MINUS: return "operatorminus";
		case UnaryOp::NOT  : return "operatornot";
	}
}

std::vector<AST*> UnaryNode::getChildren() const { return {exp}; }

AST* UnaryNode::copyTree() const 
{
   	return new UnaryNode(static_cast<ExprNode*>(exp -> copyTree()), op_type); 
}

const Type* UnaryNode::getType() const 
{
   	return call_info.callee -> return_type; 
}
bool UnaryNode::isLeftValue() const { return false; }

void UnaryNode::freeTempSpace() 
{

}

bool UnaryNode::isCompileTimeExpr() const
{
	return exp -> isCompileTimeExpr() && call_info.callee -> is_constexpr;
}

boost::optional<int> UnaryNode::getCompileTimeValue() const
{
	return boost::none;
}
	
std::string UnaryNode::toString() const 
{
	std::string res = exp -> toString();

	std::string oper;

	switch ( op_type )
	{
		case UnaryOp::PLUS : oper = "+"; break;
		case UnaryOp::MINUS: oper = "-"; break;
		case UnaryOp::NOT  : oper = "!"; break;
	}

	return oper + res;
}

void UnaryNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
