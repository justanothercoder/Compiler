#include "fornode.hpp"

ForNode::ForNode(AST *init, ExprNode *cond, AST *step, AST *stats) : init(init), cond(cond), step(step), stats(stats), for_scope(nullptr) { }

ForNode::~ForNode() 
{
	for ( auto child : getChildren() )
		delete child;

	delete for_scope;
}

void ForNode::build_scope()
{
	for_scope = new LocalScope(getScope());

	for ( auto child : getChildren() )
	{
		child->setScope(for_scope);
		child->build_scope();
	}
}

void ForNode::define(const TemplateInfo& template_info)
{
	for ( auto child : getChildren() )
		child->define(template_info);
}

void ForNode::check(const TemplateInfo& template_info)
{
	for ( auto child : getChildren() )
		child->check(template_info);
}

CodeObject& ForNode::gen(const TemplateInfo& template_info)
{
	auto label1 = ForNode::getNewLabel();	
	auto label2 = ForNode::getNewLabel();

	code_obj.emit(init->gen(template_info).getCode());
	code_obj.emit(label1 + ":");

	code_obj.emit(cond->gen(template_info).getCode());

    code_obj.emit("cmp qword [rax], 0");
    code_obj.emit("jz " + label2);

	code_obj.emit(stats->gen(template_info).getCode());
	code_obj.emit(step->gen(template_info).getCode());

	code_obj.emit("jmp " + label1);
	code_obj.emit(label2 + ":");

	return code_obj;
}

AST* ForNode::copyTree() const { return new ForNode(init->copyTree(), static_cast<ExprNode*>(cond->copyTree()), step->copyTree(), stats->copyTree()); }
vector<AST*> ForNode::getChildren() const { return {init, cond, step, stats}; }

string ForNode::getNewLabel() 
{
	static int label_num = 0;
	return "@for_label" + std::to_string(++label_num);
}
