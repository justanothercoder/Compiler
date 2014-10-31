#ifndef _ADDRNODE_HPP_
#define _ADDRNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

enum class AddrOp { REF, DEREF };

class AddrNode : public ExprNode
{

	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:

	AddrNode(ExprNode* expr, AddrOp op);
    
	AST* copyTree() const;

    const Type* getType() const override;
	bool isLeftValue() const override;

	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;
	
	std::vector<AST*> getChildren() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

private:

	ExprNode *expr;
	AddrOp op;
};

#endif
