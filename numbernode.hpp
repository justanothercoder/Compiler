#ifndef _NUMBERNODE_HPP_
#define _NUMBERNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

class NumberNode : public ExprNode
{
public:

    NumberNode(std::string num);

    void check() override;
    CodeObject& gen() override;

    AST* copyTree() const override;

	std::string getNum() const;
    
	const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

private:

	std::string num;
	CodeObject code_obj;    
};
    
#endif
