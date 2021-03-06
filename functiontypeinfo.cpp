#include "functiontypeinfo.hpp"
#include "structsymbol.hpp"
#include "logger.hpp"

FunctionTypeInfo::FunctionTypeInfo(std::vector<VariableType>           params_types) : params_types(params_types) { } 
FunctionTypeInfo::FunctionTypeInfo(std::initializer_list<VariableType> init_list   ) : params_types(init_list)    { }

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs) { return lhs.hash_func() < rhs.hash_func(); }

bool operator==(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
{
    if ( lhs.params().size() != rhs.params().size() )
        return false;

    for ( size_t i = 0; i < lhs.params().size(); ++i )
    {
        if ( lhs.paramAt(i) != rhs.paramAt(i) )
            return false;
    }

    return true;
}

std::string FunctionTypeInfo::toString() const
{
    std::string res;

    res += "(";

    if ( !params_types.empty() )
    {
        auto it = std::begin(params_types);

        res += it -> getName();
        for ( ++it; it != std::end(params_types); ++it )
            res += ", " + it -> getName();
    }

    res += ")";

    return res;
}

long long FunctionTypeInfo::hash_func() const
{
    long long res = 0;

    std::hash<std::string> hash_fn;

    for ( auto type : params_types )
        res += hash_fn(type.getName());

    return res;
}

bool FunctionTypeInfo::isCompatibleWith(const FunctionTypeInfo& info) const
{
    if ( params_types.size() != info.params_types.size() )
        return false;

    for ( size_t i = 0; i < info.params_types.size(); ++i )
    {
        if ( /*(info.params_types[i].isConst() && !params_types[i].isConst())
          || */(!info.params_types[i].base() -> isConvertableTo(params_types[i].base())) )
            return false;
    }

    return true;
}

int FunctionTypeInfo::rankOfConversion(const FunctionTypeInfo& info) const
{
    int rank = 0;

    for ( int i = 0; i < static_cast<int>(params_types.size()); ++i )
        rank += *(info.params_types[i].base() -> rankOfConversion(params_types[i].base()));

    return rank;
}

std::vector<VariableType> FunctionTypeInfo::params() const { return params_types; }
VariableType FunctionTypeInfo::paramAt(int k) const { return params_types.at(k); }
