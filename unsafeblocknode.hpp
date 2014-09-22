#ifndef _UNSAFEBLOCKNODE_HPP_
#define _UNSAFEBLOCKNODE_HPP_

#include "statementnode.hpp"

class UnsafeBlockNode : public AST
{

	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:

	UnsafeBlockNode(StatementNode *block);
	~UnsafeBlockNode();

	AST* copyTree() const override;
    
	void build_scope() override; 

    void define() override;
    void check() override;
	CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;
	
	std::string toString() const override;
	
	void accept(ASTVisitor *visitor) override;

private:

	StatementNode *block;
};

#endif
