#include "ifnode.hpp"

IfNode::IfNode(ExprNode *cond, AST *stats_true, AST *stats_false) : cond(cond), stats_true(stats_true), stats_false(stats_false), if_scope(nullptr), else_scope(nullptr), code_obj() { } 

IfNode::~IfNode()
{
	delete if_scope;
	delete else_scope;
	
	for ( auto child : getChildren() )
		delete child;
}

void IfNode::build_scope()
{
    if_scope = new LocalScope(scope);
    else_scope = new LocalScope(scope);
    
    cond->scope = scope;
    cond->build_scope();

    stats_true->scope = if_scope;
    stats_true->build_scope();

    stats_false->scope = else_scope;
    stats_false->build_scope();
}

void IfNode::define(const TemplateInfo& template_info)
{
	for ( auto child : getChildren() )
		child->define(template_info);
}
    
void IfNode::check(const TemplateInfo& template_info)
{
	for ( auto child : getChildren() )
		child->check(template_info);
}
    
CodeObject& IfNode::gen(const TemplateInfo& template_info)
{
    code_obj.emit(cond->gen(template_info).getCode());

    string false_label = IfNode::getNewLabel(), exit_label = IfNode::getNewLabel();

    code_obj.emit("cmp qword [rax], 0");
    code_obj.emit("jz " + false_label);
    
	code_obj.emit(stats_true->gen(template_info).getCode());
    
	code_obj.emit("jmp " + exit_label);
    code_obj.emit(false_label + ":");
    
	code_obj.emit(stats_false->gen(template_info).getCode());
 
 	code_obj.emit(exit_label + ":");

	return code_obj;
}

string IfNode::getNewLabel() 
{ 
	static int label_num = 0;
	return "@if_label" + std::to_string(++label_num); 
}

AST* IfNode::copyTree() const { return new IfNode(static_cast<ExprNode*>(cond->copyTree()), stats_true->copyTree(), stats_false->copyTree()); }
	
vector<AST*> IfNode::getChildren() const { return {cond, stats_true, stats_false}; }
