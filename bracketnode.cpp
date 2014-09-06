#include "bracketnode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "globalhelper.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr) 
{

}

BracketNode::~BracketNode() 
{
   	delete expr; 
}

void BracketNode::check()
{
	base -> check();

	auto base_type = dynamic_cast<const StructSymbol*>(base -> getType());

	call_info = CallHelper::callCheck("operator[]", base_type, {this -> expr});
	
	scope -> getTempAlloc().add(getType() -> getSize());
}

CodeObject& BracketNode::gen()
{
	auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(getType() -> getSize());

	code_obj.emit("lea rax, " + addr);
	code_obj.emit("push rax");

    code_obj.genCallCode(call_info, {this -> expr}, base -> gen(), base -> getType() -> isReference());
	code_obj.emit("pop rax");

	return code_obj;
}

AST* BracketNode::copyTree() const 
{ 
	return new BracketNode(
			               static_cast<ExprNode*>(base -> copyTree()), 
						   static_cast<ExprNode*>(expr -> copyTree())
						  ); 
}

std::vector<AST*> BracketNode::getChildren() const 
{ 
	return {base, expr}; 
}

const Type* BracketNode::getType() const 
{ 
	return call_info.callee -> return_type; 
}

bool BracketNode::isLeftValue() const 
{ 
	return false; 
}

void BracketNode::freeTempSpace()
{

}

bool BracketNode::isCompileTimeExpr() const
{
	return base -> isCompileTimeExpr() && expr -> isCompileTimeExpr() && call_info.callee -> is_constexpr;
}

boost::optional<int> BracketNode::getCompileTimeValue() const
{
	return boost::none;
}
