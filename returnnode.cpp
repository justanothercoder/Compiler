#include "returnnode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "exprnode.hpp"
#include "globalconfig.hpp"
#include "structsymbol.hpp"
#include "copytypevisitor.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), enclosing_func(nullptr) 
{

}

AST* ReturnNode::copyTree() const 
{
   	return new ReturnNode(static_cast<ExprNode*>(expr -> copyTree())); 
}

std::vector<AST*> ReturnNode::getChildren() const 
{
   	return {expr}; 
}

std::string ReturnNode::toString() const
{
	return "return " + expr -> toString() + ";";
}

void ReturnNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
