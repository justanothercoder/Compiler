#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr), call_info() { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::check(const TemplateInfo& template_info)
{
	base->check(template_info);

	auto base_type = dynamic_cast<StructSymbol*>(base->getType().type);
	call_info = CallHelper::callCheck("operator[]", base_type, {this->expr}, template_info);
}

void BracketNode::gen(const TemplateInfo& template_info)
{
    CodeGen::genCallCode(call_info, {this->expr}, template_info,
			[&]() { CodeGen::emit("lea rax, [" + call_info.callee->getScopedTypedName() + "]"); },
			[&]() { base->gen(template_info); }
	);
}

VariableType BracketNode::getType() const { return call_info.callee->getTypeInfo().return_type; }

AST* BracketNode::copyTree() const { return new BracketNode(static_cast<ExprNode*>(base->copyTree()), static_cast<ExprNode*>(expr->copyTree())); }
	
vector<AST*> BracketNode::getChildren() const { return {base, expr}; }
