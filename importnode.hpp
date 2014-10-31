#ifndef _IMPORTNODE_HPP_
#define _IMPORTNODE_HPP_

#include "ast.hpp"

class ImportNode : public AST
{
public:
	ImportNode(std::string lib);

    AST* copyTree() const override;
	std::string toString() const override;

	void accept(ASTVisitor& visitor) override;

	std::string lib;
};

#endif
