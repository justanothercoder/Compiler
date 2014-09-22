#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include "ast.hpp"
#include "codeobject.hpp"

class Scope;
class ExprNode;

class IfNode : public AST
{
public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

	~IfNode() override;

    AST* copyTree() const override;

    void build_scope() override;

    void define() override;
    void check() override;

    CodeObject& gen() override;
	
	std::vector<AST*> getChildren() const override;

	std::string toString() const override;

private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    Scope *if_scope;
	Scope *else_scope;

	CodeObject code_obj;

    static std::string getNewLabel();
};

#endif
