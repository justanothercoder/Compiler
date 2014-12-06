#include "variabletype.hpp"
#include "type.hpp"
#include "referencetype.hpp"

VariableType::VariableType(const Type* type, bool is_const) : type(type), is_const(is_const)
{

}

const Type* VariableType::base() const
{
    return type;
}

bool VariableType::isConst() const
{
    return is_const;
}
    
const Type* VariableType::unqualified() const
{
    if ( type -> isReference() )
        return static_cast<ReferenceType*>(type) -> type;
    else
        return type;
}
    
int VariableType::sizeOf() const
{
    return type -> getSize();
}
    
bool VariableType::isReference() const
{
    return type -> isReference();
}
    
std::string VariableType::getName() const
{
    std::string res = "";
    res += isConst() ? "const " : "";
    res += type -> getName();
    return res;
}
