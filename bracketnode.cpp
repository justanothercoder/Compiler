#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr), call_info(), code_obj() { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::check()
{
	base -> check();

	auto base_type = dynamic_cast<StructSymbol*>(base -> getType().type);

	call_info = CallHelper::callCheck("operator[]", base_type, {this -> expr});
	
	scope -> getTempAlloc().add(getType().getSize());
}

CodeObject& BracketNode::gen()
{
	string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(getType().getSize());

	code_obj.emit("lea rax, " + addr);
	code_obj.emit("push rax");

    code_obj.genCallCode(call_info, {this -> expr}, base -> gen(), base -> getType().is_ref);
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

vector<AST*> BracketNode::getChildren() const 
{ 
	return {base, expr}; 
}

VariableType BracketNode::getType() const 
{ 
	return call_info.callee -> return_type; 
}

bool BracketNode::isLeftValue() const { return false; }

int BracketNode::neededSpaceForTemporaries()
{
	return std::max(base -> neededSpaceForTemporaries(), expr -> neededSpaceForTemporaries());
}

void BracketNode::freeTempSpace()
{

}
