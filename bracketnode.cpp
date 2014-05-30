#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr), call_info(), code_obj() { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::check()
{
	base -> check();

	auto base_type = dynamic_cast<StructSymbol*>(base -> getType().type);
	call_info = CallHelper::callCheck("operator[]", base_type, {this -> expr});
	
	scope -> get_valloc() -> addReturnValueSpace(getType().getSize());
}

CodeObject& BracketNode::gen()
{
    code_obj.genCallCode(call_info, {this -> expr}, base -> gen(), base -> getType().is_ref);
	return code_obj;
}

AST* BracketNode::copyTree() const { return new BracketNode(static_cast<ExprNode*>(base -> copyTree()), static_cast<ExprNode*>(expr -> copyTree())); }
vector<AST*> BracketNode::getChildren() const { return {base, expr}; }

VariableType BracketNode::getType() const { return call_info.callee -> function_type_info.return_type; }
bool BracketNode::isLeftValue() const { return false; }

