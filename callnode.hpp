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
    
    virtual void check();
    virtual CodeObject& gen();

	virtual vector<AST*> getChildren() const;
    virtual AST* copyTree() const;

    virtual VariableType getType() const;
	virtual bool isLeftValue() const;
    
private:

    ExprNode *caller;
    vector<ExprNode*> params;

    CallInfo call_info;
	CodeObject code_obj;
};

#endif
