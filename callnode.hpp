#ifndef _CALLNODE_HPP_
#define _CALLNODE_HPP_

#include <vector>
#include <algorithm>

#include "exprnode.hpp"
#include "functionhelper.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "globalhelper.hpp"

using std::vector;

class CallNode : public ExprNode
{
public:

    CallNode(ExprNode *caller, const vector<ExprNode*>& params);
    
    virtual Type* getType() const;
    virtual bool isLeftValue() const;
    
    virtual void build_scope();
    virtual void check();
    virtual void gen();
    
private:

    ExprNode *caller;
    vector<ExprNode*> params;

    FunctionTypeInfo resolved_function_type_info;    
};

#endif
