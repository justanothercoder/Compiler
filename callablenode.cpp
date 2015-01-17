#include "callablenode.hpp"
#include "functionaltype.hpp"
#include "noviableoverloaderror.hpp"
#include "functionsymbol.hpp"

VariableType CallableNode::getType() const { return call_info.callee -> type().returnType(); }
bool CallableNode::isLeftValue() const { return getType().isReference(); }
