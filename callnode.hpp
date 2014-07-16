#ifndef _CALLNODE_HPP_
#define _CALLNODE_HPP_

#include <vector>
#include <algorithm>

#include "exprnode.hpp"
#include "functionhelper.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "globalhelper.hpp"
#include "functionsymbol.hpp"
#include "codeobject.hpp"
#include "callhelper.hpp"
#include "callinfo.hpp"

using std::vector;

class CallNode : public ExprNode
{
public:

    CallNode(ExprNode *caller, const vector<ExprNode*>& params);
	~CallNode() override;
    
    void check() override;
    CodeObject& gen() override;

	vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    VariableType getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
    
	bool isCompileTimeExpr() const override;
	optional<int> getCompileTimeValue() const override;

private:

    ExprNode *caller;
    vector<ExprNode*> params;

    CallInfo call_info;
	CodeObject code_obj;
};

#endif
