#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr), call_info(), code_obj() { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::check(const TemplateInfo& template_info)
{
	base->check(template_info);

	auto base_type = dynamic_cast<StructSymbol*>(base->getType().type);
	call_info = CallHelper::callCheck("operator[]", base_type, {this->expr}, template_info);
}

CodeObject& BracketNode::gen(const TemplateInfo& template_info)
{
    code_obj.genCallCode(call_info, {this->expr}, template_info, base->gen(template_info));
	return code_obj;
}

AST* BracketNode::copyTree() const { return new BracketNode(static_cast<ExprNode*>(base->copyTree()), static_cast<ExprNode*>(expr->copyTree())); }
vector<AST*> BracketNode::getChildren() const { return {base, expr}; }

VariableType BracketNode::getType() const { return call_info.callee->getTypeInfo().return_type; }
bool BracketNode::isLeftValue() const { return false; }

