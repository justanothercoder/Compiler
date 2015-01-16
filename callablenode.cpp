#include "callablenode.hpp"
#include "functionaltype.hpp"
#include "noviableoverloaderror.hpp"

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


