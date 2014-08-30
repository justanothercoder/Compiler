#ifndef _NEWEXPRESSIONNODE_HPP_
#define _NEWEXPRESSIONNODE_HPP_

#include <vector>
#include <string>
#include <algorithm>

#include "exprnode.hpp"
#include "codeobject.hpp"

class NewExpressionNode : public ExprNode
{
public:
    NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params);
	~NewExpressionNode() override;

	void build_scope() override;
    void check() override;
    CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

	bool isCompileTimeExpr() const override; 
	boost::optional<int> getCompileTimeValue() const override;

private:

    TypeInfo type_info;
	std::vector<ExprNode*> params;
    CallInfo call_info;
	CodeObject code_obj;
};

#endif
