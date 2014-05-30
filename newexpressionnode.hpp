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
	virtual ~NewExpressionNode();

    virtual void check();
    virtual CodeObject& gen();

	virtual vector<AST*> getChildren() const;
    virtual AST* copyTree() const;

    virtual VariableType getType() const;
	virtual bool isLeftValue() const;

private:

    TypeInfo type_info;
    vector<ExprNode*> params;
    CallInfo call_info;
	CodeObject code_obj;
};

#endif
