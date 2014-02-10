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

void WhileNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	while_scope->recalc_scope_address();

    cond->check(template_sym, expr);
    stats->check(template_sym, expr);
}

string WhileNode::getNewLabel() { return "@while_label" + std::to_string(++WhileNode::label_num); }

void WhileNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { stats->define(template_sym, expr); }
    
AST* WhileNode::copyTree() const { return new WhileNode(static_cast<ExprNode*>(cond->copyTree()), stats->copyTree()); }

void WhileNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    string exit_label = WhileNode::getNewLabel(), cycle_label = WhileNode::getNewLabel();
    
    CodeGen::emit(cycle_label + ":");
    cond->gen(template_sym, expr);
    CodeGen::emit("cmp qword [rax], 0");
    CodeGen::emit("jz " + exit_label);
    stats->gen(template_sym, expr);
    CodeGen::emit("jmp " + cycle_label);
    CodeGen::emit(exit_label + ":");
}
