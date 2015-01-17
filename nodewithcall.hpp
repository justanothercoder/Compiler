#ifndef _NODEWITHCALL_HPP_
#define _NODEWITHCALL_HPP_

#include "callinfo.hpp"
#include "inlineinfo.hpp"

class NodeWithCall
{
public:

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const InlineInfo& inlineInfo() const;
    void inlineInfo(InlineInfo inline_info);

protected:
    
    CallInfo call_info;
    InlineInfo inline_info;
};

#endif
