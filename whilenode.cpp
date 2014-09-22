#include "whilenode.hpp"
#include "exprnode.hpp"
#include "localscope.hpp"

WhileNode::WhileNode(ExprNode *cond, AST *stats) : cond(cond), stats(stats), while_scope(nullptr)
{

}

WhileNode::~WhileNode() 
{ 
	delete cond; 
	delete stats; 
	delete while_scope; 
}

void WhileNode::build_scope()
{
    while_scope = new LocalScope(scope);

    cond -> scope = scope;
    cond -> build_scope();

    stats -> scope = while_scope;
    stats -> build_scope();
}

void WhileNode::define() { stats -> define(); }

void WhileNode::check()
{
    cond  -> check();
    stats -> check();
}

CodeObject& WhileNode::gen()
{
    auto exit_label  = WhileNode::getNewLabel();
    auto cycle_label = WhileNode::getNewLabel();
    
    code_obj.emit(cycle_label + ":");
    code_obj.emit(cond -> gen().getCode());
    code_obj.emit("cmp qword [rax], 0");
    code_obj.emit("jz " + exit_label);
    code_obj.emit(stats -> gen().getCode());
    code_obj.emit("jmp " + cycle_label);
    code_obj.emit(exit_label + ":");

	return code_obj;
}

std::string WhileNode::getNewLabel() 
{
	static int label_num = 0;
	return "@while_label" + std::to_string(++label_num); 
}

AST* WhileNode::copyTree() const 
{
   	return new WhileNode(static_cast<ExprNode*>(cond -> copyTree()), stats -> copyTree()); 
}
   	
std::vector<AST*> WhileNode::getChildren() const 
{
   	return {cond, stats}; 
}
	
std::string WhileNode::toString() const 
{
	return "while (" + cond -> toString() + ")\n" + stats -> toString();
}

void WhileNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
