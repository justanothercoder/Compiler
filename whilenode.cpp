#include "whilenode.hpp"

WhileNode::WhileNode(ExprNode *cond, AST *stats) : cond(cond), stats(stats), while_scope(nullptr) { }

WhileNode::~WhileNode() 
{ 
	delete cond; 
	delete stats; 
	delete while_scope; 
}

void WhileNode::build_scope()
{
    while_scope = new LocalScope(getScope());

    cond->setScope(getScope());
    cond->build_scope();

    stats->setScope(while_scope);
    stats->build_scope();
}

void WhileNode::define(const TemplateInfo& template_info) { stats->define(template_info); }

void WhileNode::check(const TemplateInfo& template_info)
{
    cond->check(template_info);
    stats->check(template_info);
}

void WhileNode::gen(const TemplateInfo& template_info)
{
    string exit_label = WhileNode::getNewLabel(), cycle_label = WhileNode::getNewLabel();
    
    CodeGen::emit(cycle_label + ":");
    cond->gen(template_info);
    CodeGen::emit("cmp qword [rax], 0");
    CodeGen::emit("jz " + exit_label);
    stats->gen(template_info);
    CodeGen::emit("jmp " + cycle_label);
    CodeGen::emit(exit_label + ":");
}

string WhileNode::getNewLabel() 
{
	static int label_num = 0;
	return "@while_label" + std::to_string(++label_num); 
}

AST* WhileNode::copyTree() const { return new WhileNode(static_cast<ExprNode*>(cond->copyTree()), stats->copyTree()); }
   	
vector<AST*> WhileNode::getChildren() const { return {cond, stats}; }
