#include "ifnode.hpp"

IfNode::IfNode(ExprNode *cond, AST *stats_true, AST *stats_false) : cond(cond), stats_true(stats_true), stats_false(stats_false), if_scope(nullptr), else_scope(nullptr) { } 

IfNode::~IfNode()
{
	delete if_scope;
	delete else_scope;
	
	for ( auto child : getChildren() )
		delete child;
}

void IfNode::build_scope()
{
    if_scope = new LocalScope(getScope());
    else_scope = new LocalScope(getScope());
    
    cond->setScope(getScope());
    cond->build_scope();

    stats_true->setScope(if_scope);
    stats_true->build_scope();

    stats_false->setScope(else_scope);
    stats_false->build_scope();
}

void IfNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto child : getChildren() )
		child->define(template_sym, expr);
}
    
void IfNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto child : getChildren() )
		child->check(template_sym, expr);
}
    
void IfNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    cond->gen(template_sym, expr);

    string false_label = IfNode::getNewLabel(), exit_label = IfNode::getNewLabel();

    CodeGen::emit("cmp qword [rax], 0");
    CodeGen::emit("jz " + false_label);
    
	stats_true->gen(template_sym, expr);
    
	CodeGen::emit("jmp " + exit_label);
    CodeGen::emit(false_label + ":");
    
	stats_false->gen(template_sym, expr);
 
 	CodeGen::emit(exit_label + ":");
}

string IfNode::getNewLabel() 
{ 
	static int label_num = 0;
	return "@if_label" + std::to_string(++label_num); 
}

AST* IfNode::copyTree() const { return new IfNode(static_cast<ExprNode*>(cond->copyTree()), stats_true->copyTree(), stats_false->copyTree()); }
	
vector<AST*> IfNode::getChildren() const { return {cond, stats_true, stats_false}; }
