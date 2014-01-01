#ifndef _CALLNODE_HPP_
#define _CALLNODE_HPP_

#include <vector>
#include <algorithm>

#include "exprnode.hpp"
#include "functiontype.hpp"
#include "functionhelper.hpp"

using std::vector;

class CallNode : public ExprNode
{
public:

    CallNode(ExprNode *caller, const vector<ExprNode*>& params);
    
    virtual Type* getType();
    virtual bool isLeftValue();
    
    virtual void build_scope();
    virtual void check();
    virtual void gen();
    
private:

    FunctionType *resolved_function_type;
    
    ExprNode *caller;
    vector<ExprNode*> params;
};

#endif
