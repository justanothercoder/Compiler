#include "fornode.hpp"
#include "localscope.hpp"

ForNode::ForNode(AST *init, ExprNode *cond, AST *step, AST *stats) : init(init), cond(cond), step(step), stats(stats), for_scope(nullptr) 
{

}

ForNode::~ForNode() 
{
	for ( auto child : getChildren() )
		delete child;

	delete for_scope;
}

void ForNode::build_scope()
{
	for_scope = new LocalScope(scope);

	for ( auto child : getChildren() )
	{
		child -> scope = for_scope;
		child -> build_scope();
	}
}

CodeObject& ForNode::gen()
{
	auto label1 = ForNode::getNewLabel();	
	auto label2 = ForNode::getNewLabel();

	code_obj.emit(init -> gen().getCode());
	code_obj.emit(label1 + ":");

	code_obj.emit(cond -> gen().getCode());

    code_obj.emit("cmp qword [rax], 0");
    code_obj.emit("jz " + label2);

	code_obj.emit(stats -> gen().getCode());
	code_obj.emit(step -> gen().getCode());

	code_obj.emit("jmp " + label1);
	code_obj.emit(label2 + ":");

	return code_obj;
}

AST* ForNode::copyTree() const 
{ 
	return new ForNode(init -> copyTree(), static_cast<ExprNode*>(cond -> copyTree()), step -> copyTree(), stats -> copyTree()); 
}

std::vector<AST*> ForNode::getChildren() const 
{
   	return {init, cond, step, stats}; 
}

std::string ForNode::getNewLabel() 
{
	static int label_num = 0;
	return "@for_label" + std::to_string(++label_num);
}
	
std::string ForNode::toString() const 
{
	return "for (" + init -> toString() + "; " + cond -> toString() + "; " + step -> toString() + ")\n" + stats -> toString();
}

void ForNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
