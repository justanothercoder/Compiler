#ifndef _NEWEXPRESSIONNODE_HPP_
#define _NEWEXPRESSIONNODE_HPP_

#include <vector>
#include <string>
#include <algorithm>

#include "exprnode.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "typehelper.hpp"
#include "codeobject.hpp"
#include "callhelper.hpp"

using std::vector;
using std::string;

class NewExpressionNode : public ExprNode
{
public:
    NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params);
	~NewExpressionNode() override;

    void check() override;
    CodeObject& gen() override;

	vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    VariableType getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

private:

    TypeInfo type_info;
    vector<ExprNode*> params;
    CallInfo call_info;
	CodeObject code_obj;
};

#endif
