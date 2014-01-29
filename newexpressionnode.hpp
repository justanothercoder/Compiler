#ifndef _NEWEXPRESSIONNODE_HPP_
#define _NEWEXPRESSIONNODE_HPP_

#include <vector>
#include <string>
#include <algorithm>

#include "exprnode.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "typehelper.hpp"
#include "codegen.hpp"

using std::vector;
using std::string;

class NewExpressionNode : public ExprNode
{
public:
    NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params);

    virtual void build_scope();
    virtual void check();
    virtual void gen();

    virtual Type* getType() const;
    virtual bool isLeftValue() const;

    virtual void template_check();

private:

    TypeInfo type_info;
    vector<ExprNode*> params;
    FunctionSymbol *resolved_constructor;
};

#endif
