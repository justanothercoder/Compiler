#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), func(nullptr) { }

void ReturnNode::build_scope()
{
    expr->setScope(this->getScope());
    expr->build_scope();
}

void ReturnNode::define() { }

void ReturnNode::check()
{
    Scope *sc = this->getScope();
    while ( sc != nullptr && dynamic_cast<FunctionSymbol*>(sc) == nullptr )
	sc = sc->getEnclosingScope();

    if ( sc == nullptr )
	throw SemanticError("return is not a in a function");

    func = dynamic_cast<FunctionSymbol*>(sc);

    expr->check();    
}

void ReturnNode::gen()
{
    expr->gen();
    for ( int i = 0; i < expr->getType()->getSize(); i += GlobalConfig::int_size )
    {
	CodeGen::emit("mov rbx, [rax - " + std::to_string(i) +  "]");
	CodeGen::emit("mov [rsp - " + std::to_string(i + GlobalConfig::int_size) +  "], rbx");
    }
    CodeGen::emit("lea rax, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");

    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
}

void ReturnNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& _expr)
{
    expr->template_check(template_sym, _expr);
}

bool ReturnNode::isTemplated() const
{
    return expr->isTemplated();
}

void ReturnNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    this->expr->template_define(template_sym, expr);
}

AST* ReturnNode::copyTree() const
{
    return new ReturnNode(static_cast<ExprNode*>(expr->copyTree()));
}

void ReturnNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}
