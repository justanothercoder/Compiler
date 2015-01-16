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

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const InlineInfo& inlineInfo() const;
    void inlineInfo(InlineInfo inline_info);

protected:

    CallInfo call_info;
    InlineInfo inline_info;
};

#endif
