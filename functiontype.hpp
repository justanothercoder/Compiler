#ifndef _FUNCTIONTYPE_HPP_
#define _FUNCTIONTYPE_HPP_

#include "type.hpp"
#include "functiontypeinfo.hpp"

class FunctionType
{
public:

    FunctionType(VariableType return_type, FunctionTypeInfo type_info);

    std::string getName() const;

    VariableType returnType() const;
    FunctionTypeInfo typeInfo() const;

private:

    VariableType return_type;
    FunctionTypeInfo type_info;
};

#endif
