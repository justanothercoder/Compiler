#ifndef _ADDRNODE_HPP_
#define _ADDRNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

enum class AddrOp { REF, DEREF };

class AddrNode : public ExprNode
{
public:

	AddrNode(ExprNode* expr, AddrOp op);
    
	void check() override;
	CodeObject& gen() override;
    
	AST* copyTree() const;

    const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;
	
	std::vector<AST*> getChildren() const override;

	std::string toString() const override;

private:

	ExprNode *expr;
	AddrOp op;

	CodeObject code_obj;
};

#endif
