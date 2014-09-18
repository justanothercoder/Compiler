#ifndef _STRINGNODE_HPP_
#define _STRINGNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

class StringNode : public ExprNode
{
public:

	StringNode(std::string str);

    void check() override;
    CodeObject& gen() override;

	AST* copyTree() const override;

	std::string getStr() const;
	static std::string getNewLabel();

	const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor *visitor) override;

private:
	
	std::string str;
	CodeObject code_obj;
};

#endif
