#ifndef _NULLNODE_HPP_
#define _NULLNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

class NullNode : public ExprNode
{
public:

	NullNode();
    
	AST* copyTree() const override;
	
	std::string toString() const override;
    
	const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;
	
	void accept(ASTVisitor& visitor) override;
};

#endif
