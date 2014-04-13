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

	virtual ~CallNode();
    
    virtual AST* copyTree() const;

    virtual VariableType getType() const;
    
    virtual void check(const TemplateInfo& template_info);
    virtual CodeObject& gen(const TemplateInfo& template_info);

	virtual vector<AST*> getChildren() const;
    
private:

    ExprNode *caller;
    vector<ExprNode*> params;

    CallInfo call_info;
	CodeObject code_obj;
};

#endif
