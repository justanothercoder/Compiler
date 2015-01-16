#ifndef _CALLABLENODE_HPP_
#define _CALLABLENODE_HPP_

#include "exprnode.hpp"
#include "callinfo.hpp"
#include "inlineinfo.hpp"
#include "valueinfo.hpp"

class FunctionalType;

class CallableNode : public ExprNode
{
public:

    virtual const FunctionalType* function() const = 0;
    virtual std::vector<ValueInfo> arguments() const = 0;

    VariableType getType() const override;
    bool isLeftValue() const override;

    void checkCall();
    void genCall();

protected:

    CallInfo call_info;
    InlineInfo inline_info;
};

#endif
