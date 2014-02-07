#include "ifnode.hpp"

int IfNode::label_num = 0;

IfNode::IfNode(ExprNode *cond, AST *stats_true, AST *stats_false) : cond(cond), stats_true(stats_true), stats_false(stats_false)
{
    if_scope = nullptr;
    else_scope = nullptr;
}

void IfNode::build_scope()
{
    if_scope = new LocalScope(this->getScope());
    else_scope = new LocalScope(this->getScope());
    
    cond->setScope(this->getScope());
    cond->build_scope();

    stats_true->setScope(if_scope);
    stats_true->build_scope();

    stats_false->setScope(else_scope);
    stats_false->build_scope();
}

void IfNode::define()
{
    stats_true->define();
    stats_false->define();
}

void IfNode::check()
{
    cond->check();

    if_scope->recalc_scope_address();
    stats_true->check();

    else_scope->recalc_scope_address();
    stats_false->check();    
}
    
void IfNode::gen()
{
    cond->gen();

    string false_label = IfNode::getNewLabel(), exit_label = IfNode::getNewLabel();

    CodeGen::emit("cmp qword [rax], 0");
    CodeGen::emit("jz " + false_label);
    stats_true->gen();
    CodeGen::emit("jmp " + exit_label);
    CodeGen::emit(false_label + ":");
    stats_false->gen();
    CodeGen::emit(exit_label + ":");
}

string IfNode::getNewLabel()
{
    return "@if_label" + std::to_string(++IfNode::label_num);
}
    
void IfNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    cond->template_check(template_sym, expr);
    stats_true->template_check(template_sym, expr);
    stats_false->template_check(template_sym, expr);
}

bool IfNode::isTemplated() const
{
    return cond->isTemplated() || stats_true->isTemplated() || stats_false->isTemplated();
}

void IfNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}

AST* IfNode::copyTree() const
{
    
}

void IfNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}
