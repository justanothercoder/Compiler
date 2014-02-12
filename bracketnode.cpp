#include "bracketnode.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr) { }

BracketNode::~BracketNode() { delete expr; }

void BracketNode::build_scope()
{
	base->setScope(this->getScope());
	base->build_scope();

	expr->setScope(this->getScope());
	expr->build_scope();
}

void BracketNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	base->check(template_sym, expr);
	this->expr->check(template_sym, expr);

	StructSymbol *base_type = dynamic_cast<StructSymbol*>(static_cast<ReferenceType*>(base->getType())->getReferredType());
	
	Symbol *_op = base_type->resolve("operator[]");

	if ( _op == nullptr )
		throw SemanticError("No such operator[] in " + base_type->getName());

	VariableSymbol *op = dynamic_cast<VariableSymbol*>(_op);

	if ( op == nullptr )
		throw SemanticError("No such operator[] in " + base_type->getName());

	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(op->getType());

	resolved_operator = FunctionHelper::getViableOverload(ov_func, {this->base->getType(), this->expr->getType()});

	if ( resolved_operator == nullptr )
		throw SemanticError("No viable overload");
}

void BracketNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    string call_name = resolved_operator->getEnclosingScope()->getScopeName() + "_";

    call_name += "operatorelem";
    call_name += resolved_operator->getTypedName().substr(string("operatorelem").length());

    CodeGen::emit("push rsi");
    CodeGen::emit("lea rsi, [" + call_name + "]");

    CodeGen::genCallCode(resolved_operator, {this->expr}, template_sym, expr);
    CodeGen::emit("pop rsi");
}

Type* BracketNode::getType() const { return resolved_operator->getTypeInfo().getReturnType(); }

bool BracketNode::isLeftValue() const { return resolved_operator->getTypeInfo().getReturnType()->getTypeKind() == TypeKind::REFERENCE; }

AST* BracketNode::copyTree() const { return new BracketNode(static_cast<ExprNode*>(base->copyTree()), static_cast<ExprNode*>(expr->copyTree())); }
