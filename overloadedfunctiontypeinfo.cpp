#include "overloadedfunctiontypeinfo.hpp"
#include "logger.hpp"

OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(std::set<FunctionTypeInfo> overloads) : overloads(overloads)
{

}

std::set<FunctionTypeInfo> OverloadedFunctionTypeInfo::getPossibleOverloads(FunctionTypeInfo params_type) const
{
    auto possible = overloads;

    for ( auto i = std::begin(possible); i != std::end(possible); )
    {
        if ( !i -> isCompatibleWith(params_type) )
            i = possible.erase(i);
        else
            ++i;
    }

    return possible;
}
