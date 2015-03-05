#include "typesymbol.hpp"
#include "varsymbol.hpp"
#include "functionalsymbol.hpp"
#include "typefactory.hpp"

#include "logger.hpp"

const Symbol* TypeSymbol::resolveMember(std::string name) const 
{
    for ( auto mem : members() )
    {
        if ( mem -> getName() == name )
            return mem;
    }

    return nullptr;
}

const FunctionalSymbol* TypeSymbol::resolveMethod(std::string name, const FunctionTypeInfo& info) const 
{
    auto params = std::vector<VariableType>{TypeFactory::getReference(this)};
    auto arguments = info.params();
    params.insert(std::end(params), std::begin(arguments), std::end(arguments));

    for ( auto meth : methods() )
    {
        if ( meth -> getName() == name && meth -> isCompatibleWith(params) )
            return meth;
    }

    return nullptr;
}
