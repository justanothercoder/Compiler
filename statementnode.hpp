#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>
#include <algorithm>

#include "ast.hpp"

class StatementNode : public AST
{   
public:

    StatementNode(std::vector<AST*> statements);

    AST* copyTree() const override;

	std::vector<AST*> getChildren() const override;
	
	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

	std::vector<AST*> statements;
};

#endif
