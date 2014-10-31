#include "ifnode.hpp"
#include "localscope.hpp"
#include "exprnode.hpp"

IfNode::IfNode(ExprNode *cond, AST *stats_true, AST *stats_false) : cond(cond)
																  , stats_true(stats_true)
																  , stats_false(stats_false)
																  , if_scope(nullptr)
																  , else_scope(nullptr)
{

} 

void IfNode::build_scope()
{
    if_scope   = std::make_shared<LocalScope>(scope);
    else_scope = std::make_shared<LocalScope>(scope);
    
    cond -> scope = scope;
    cond -> build_scope();

    stats_true -> scope = if_scope.get();
    stats_true -> build_scope();

    stats_false -> scope = else_scope.get();
    stats_false -> build_scope();
}

std::string IfNode::getNewLabel() 
{ 
	static int label_num = 0;
	return "@if_label" + std::to_string(++label_num); 
}

AST* IfNode::copyTree() const 
{ 
	return new IfNode(static_cast<ExprNode*>(cond -> copyTree()), stats_true -> copyTree(), stats_false -> copyTree()); 
}
	
std::vector<AST*> IfNode::getChildren() const 
{
   	return {cond, stats_true, stats_false}; 
}
	
std::string IfNode::toString() const
{
	std::string res = "if (" + cond -> toString() + ")\n";
	
	res += stats_true -> toString();

	if ( stats_false )
		res += "else\n" + stats_false -> toString();

	return res;
}

void IfNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
