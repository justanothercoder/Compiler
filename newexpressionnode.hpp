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
#include "callhelper.hpp"

using std::vector;
using std::string;

class NewExpressionNode : public ExprNode
{
public:
    NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params);

	virtual ~NewExpressionNode();

    virtual AST* copyTree() const;

    virtual VariableType getType() const;

    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;
    
private:

    TypeInfo type_info;
    vector<ExprNode*> params;
    CallInfo call_info;
};

#endif
