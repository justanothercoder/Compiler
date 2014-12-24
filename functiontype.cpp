#include "functiontype.hpp"
#include "globalconfig.hpp"

FunctionType::FunctionType(VariableType return_type, FunctionTypeInfo type_info) : return_type(return_type), type_info(type_info)
{

}

std::string FunctionType::getName() const
{
    std::string res = "";

    res += return_type.getName();
    res += "(";
    
    if ( !type_info.params_types.empty() )
    {
        auto it = std::begin(type_info.params_types);
        res += it -> getName();
        for ( ++it; it != std::end(type_info.params_types); ++it ) 
            res += ", " + it -> getName();
    }

    res += ")";
    return res;
}
    
VariableType FunctionType::returnType() const { return return_type; } 
const FunctionTypeInfo& FunctionType::typeInfo() const { return type_info; }
