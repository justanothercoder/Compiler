#include "nodewithcall.hpp"

const CallInfo& NodeWithCall::callInfo() const { return call_info; }
void NodeWithCall::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }

const InlineInfo& NodeWithCall::inlineInfo() const { return inline_info; }
void NodeWithCall::inlineInfo(InlineInfo inline_info) { this -> inline_info = std::move(inline_info); }
