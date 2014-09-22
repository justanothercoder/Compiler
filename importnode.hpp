#ifndef _IMPORTNODE_HPP_
#define _IMPORTNODE_HPP_

#include "ast.hpp"
#include "codeobject.hpp"

class ImportNode : public AST
{
public:
	ImportNode(std::string lib);

    AST* copyTree() const override;
	
    void define() override;
    void check() override;

    CodeObject& gen() override;
	std::string toString() const override;

	void accept(ASTVisitor& visitor) override;

private:

	std::string lib;
	CodeObject code_obj;
};

#endif
