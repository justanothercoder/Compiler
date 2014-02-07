#include "whilenode.hpp"

int WhileNode::label_num = 0;

WhileNode::WhileNode(ExprNode *cond, AST *stats) : cond(cond), stats(stats), while_scope(nullptr) { }

void WhileNode::build_scope()
{
    while_scope = new LocalScope(getScope());

    cond->setScope(getScope());
    cond->build_scope();

    stats->setScope(while_scope);
    stats->build_scope();
}

void WhileNode::define() { stats->define(); }

void WhileNode::check()
{
    while_scope->recalc_scope_address();

    cond->check();
    stats->check();
}

void WhileNode::gen()
{
    string exit_label = WhileNode::getNewLabel(), cycle_label = WhileNode::getNewLabel();
    
    CodeGen::emit(cycle_label + ":");
    cond->gen();
    CodeGen::emit("cmp qword [rax], 0");
    CodeGen::emit("jz " + exit_label);
    stats->gen();
    CodeGen::emit("jmp " + cycle_label);
    CodeGen::emit(exit_label + ":");
}

string WhileNode::getNewLabel() { return "@while_label" + std::to_string(++WhileNode::label_num); }

void WhileNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    cond->template_check(template_sym, expr);
    stats->template_check(template_sym, expr);
}

bool WhileNode::isTemplated() const
{
    return cond->isTemplated() || stats->isTemplated();
}

void WhileNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}

AST* WhileNode::copyTree() const
{
    
}

void WhileNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}
