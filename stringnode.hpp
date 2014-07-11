#ifndef _STRINGNODE_HPP_
#define _STRINGNODE_HPP_

#include "exprnode.hpp"
#include "builtins.hpp"
#include "codeobject.hpp"

class StringNode : public ExprNode
{
public:

	StringNode(string str);

    void check() override;
    CodeObject& gen() override;

	AST* copyTree() const override;

	string getStr() const;
	static string getNewLabel();

	VariableType getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

private:
	
	string str;
	CodeObject code_obj;
};

#endif
