#include "overloadedfunctiontypeinfo.hpp"
#include "logger.hpp"

OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(std::set<FunctionTypeInfo> overloads) : overloads(overloads)
{

}

std::set<FunctionTypeInfo> OverloadedFunctionTypeInfo::getPossibleOverloads(FunctionTypeInfo params_type) const
{
    auto possible = overloads;

    for ( auto it = std::begin(possible); it != std::end(possible); )
    {
        if ( it -> isCompatibleWith(params_type) )
            ++it;
        else
            it = possible.erase(it);
    }

    return possible;
}
