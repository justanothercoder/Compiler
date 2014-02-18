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
#include "codegen.hpp"
#include "callhelper.hpp"

using std::vector;

class CallNode : public ExprNode
{
public:

    CallNode(ExprNode *caller, const vector<ExprNode*>& params);

	virtual ~CallNode();
    
    virtual AST* copyTree() const;

    virtual Type* getType() const;
    
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;
    
private:

    ExprNode *caller;
    vector<ExprNode*> params;

    FunctionSymbol *resolved_function_symbol;    
};

#endif
