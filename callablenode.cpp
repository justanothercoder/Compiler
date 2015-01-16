#include "callablenode.hpp"
#include "functionaltype.hpp"
#include "noviableoverloaderror.hpp"
#include "functionsymbol.hpp"

void CallableNode::checkCall()
{
    assert(function() != nullptr);
    try
    {
        call_info = function() -> resolveCall(arguments());
    }
    catch ( NoViableOverloadError& e ) { throw NoViableOverloadError(function() -> getName(), arguments()); }
}

void CallableNode::genCall()
{

}

VariableType CallableNode::getType() const { return call_info.callee -> type().returnType(); }
bool CallableNode::isLeftValue() const { return getType().isReference(); }

