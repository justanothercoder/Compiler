#include "callablenode.hpp"
#include "functionaltype.hpp"
#include "noviableoverloaderror.hpp"
#include "functionsymbol.hpp"

VariableType CallableNode::getType() const { return call_info.callee -> type().returnType(); }
bool CallableNode::isLeftValue() const { return getType().isReference(); }

const CallInfo& CallableNode::callInfo() const { return call_info; }
void CallableNode::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }

const InlineInfo& CallableNode::inlineInfo() const { return inline_info; }
void CallableNode::inlineInfo(InlineInfo inline_info) { this -> inline_info = std::move(inline_info); }
