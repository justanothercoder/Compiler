#ifndef _CALLNODE_HPP_
#define _CALLNODE_HPP_

#include <vector>
#include <algorithm>

#include "exprnode.hpp"
#include "codeobject.hpp"
#include "callinfo.hpp"

class CallNode : public ExprNode
{

	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:

    CallNode(ExprNode *caller, std::vector<ExprNode*> params);
	~CallNode() override;
    
    CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
    
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

private:

    ExprNode *caller;
	std::vector<ExprNode*> params;

    CallInfo call_info;
	CodeObject code_obj;
};

#endif
