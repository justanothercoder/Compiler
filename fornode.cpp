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

void ForNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto child : getChildren() )
		child->define(template_sym, expr);
}

void ForNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto child : getChildren() )
		child->check(template_sym, expr);
}

void ForNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	auto label1 = ForNode::getNewLabel();	
	auto label2 = ForNode::getNewLabel();

	init->gen(template_sym, expr);
	CodeGen::emit(label1 + ":");

	cond->gen(template_sym, expr);

    CodeGen::emit("cmp qword [rax], 0");
    CodeGen::emit("jz " + label2);

	stats->gen(template_sym, expr);
	step->gen(template_sym, expr);

	CodeGen::emit("jmp " + label1);
	CodeGen::emit(label2 + ":");
}

AST* ForNode::copyTree() const { return new ForNode(init->copyTree(), static_cast<ExprNode*>(cond->copyTree()), step->copyTree(), stats->copyTree()); }
vector<AST*> ForNode::getChildren() const { return {init, cond, step, stats}; }

string ForNode::getNewLabel() 
{
	static int label_num = 0;
	return "@for_label" + std::to_string(++label_num);
}
