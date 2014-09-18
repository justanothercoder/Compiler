#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include "ast.hpp"
#include "codeobject.hpp"

class Scope;
class ExprNode;

class WhileNode : public AST
{
public:

    WhileNode(ExprNode *cond, AST *stats);

	~WhileNode() override;

    AST* copyTree() const override;

    void build_scope() override;

    void define() override;
    void check() override;

    CodeObject& gen() override;
   
	std::vector<AST*> getChildren() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor *visitor) override;
    
private:

    ExprNode *cond;
    AST *stats;

    Scope *while_scope;

	CodeObject code_obj;

    static std::string getNewLabel();
};

#endif
