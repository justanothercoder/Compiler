#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr), resolved_operator(nullptr) { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	base->check(template_sym, expr);

	StructSymbol *base_type = dynamic_cast<StructSymbol*>(TypeHelper::removeReference(base->getType()));
	resolved_operator = CallHelper::callCheck("operator[]", base_type, {this->expr}, template_sym, expr);
}

void BracketNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    string call_name = resolved_operator->getEnclosingScope()->getScopeName() + "_";

    call_name += "operatorelem";
    call_name += resolved_operator->getTypedName().substr(string("operatorelem").length());

    CodeGen::emit("push rsi");
    CodeGen::emit("lea rsi, [" + call_name + "]");

	base->gen(template_sym, expr);
	CodeGen::emit("push rdi");
	CodeGen::emit("mov rdi, rax");

    CodeGen::genCallCode(resolved_operator, {this->expr}, template_sym, expr);
	CodeGen::emit("pop rdi");
	CodeGen::emit("pop rsi");
}

Type* BracketNode::getType() const { return resolved_operator->getTypeInfo().getReturnType(); }

AST* BracketNode::copyTree() const { return new BracketNode(static_cast<ExprNode*>(base->copyTree()), static_cast<ExprNode*>(expr->copyTree())); }
	
vector<AST*> BracketNode::getChildren() const { return {base, expr}; }
