#include "functiontype.hpp"
#include "globalconfig.hpp"

FunctionType::FunctionType(VariableType return_type, FunctionTypeInfo type_info) : return_type(return_type), type_info(type_info)
{

}

std::string FunctionType::getName() const { return return_type.getName() + type_info.toString(); } 
VariableType FunctionType::returnType() const { return return_type; } 
const FunctionTypeInfo& FunctionType::typeInfo() const { return type_info; }
