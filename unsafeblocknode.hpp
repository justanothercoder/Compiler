#ifndef _UNSAFEBLOCKNODE_HPP_
#define _UNSAFEBLOCKNODE_HPP_

#include "statementnode.hpp"

class UnsafeBlockNode : public AST
{
public:

	UnsafeBlockNode(StatementNode *block);

	AST* copyTree() const override;
    
	void build_scope() override; 

	std::vector<AST*> getChildren() const override;
	
	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

	StatementNode *block;
};

#endif
