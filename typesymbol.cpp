#include "typesymbol.hpp"
#include "varsymbol.hpp"
#include "functionalsymbol.hpp"

const Symbol* TypeSymbol::resolveMember(const std::string& name) const 
{
    for ( auto mem : members() )
    {
        if ( mem -> getName() == name )
            return mem;
    }

    return nullptr;
}

const FunctionalSymbol* TypeSymbol::resolveMethod(const std::string& name) const 
{
    for ( auto meth : methods() )
    {
        if ( meth -> getName() == name )
            return meth;
    }

    return nullptr;
}
