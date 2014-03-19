#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr), call_info() { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	base->check(template_sym, expr);

	auto base_type = dynamic_cast<StructSymbol*>(base->getType().type);
	call_info = CallHelper::callCheck("operator[]", base_type, {this->expr}, template_sym, expr);
}

void BracketNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    CodeGen::genCallCode(call_info, {this->expr}, template_sym, expr,
			[&]() { CodeGen::emit("lea rax, [" + call_info.callee->getScopedTypedName() + "]"); },
			[&]() { base->gen(template_sym, expr); }
	);
}

VariableType BracketNode::getType() const { return call_info.callee->getTypeInfo().return_type; }

AST* BracketNode::copyTree() const { return new BracketNode(static_cast<ExprNode*>(base->copyTree()), static_cast<ExprNode*>(expr->copyTree())); }
	
vector<AST*> BracketNode::getChildren() const { return {base, expr}; }
