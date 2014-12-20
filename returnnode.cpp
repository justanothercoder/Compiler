#include "returnnode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "exprnode.hpp"
#include "globalconfig.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), enclosing_func(nullptr), is_in_inline_call(false) { }

AST* ReturnNode::copyTree() const { return new ReturnNode(static_cast<ExprNode*>(expr -> copyTree())); } 

std::vector<AST*> ReturnNode::getChildren() const { return {expr}; } 

std::string ReturnNode::toString() const { return "return " + expr -> toString() + ";"; } 

void ReturnNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
