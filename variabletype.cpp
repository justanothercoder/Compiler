#include "variabletype.hpp"
#include "type.hpp"
#include "referencetype.hpp"

VariableType::VariableType(const Type* type) : type(type), is_const(false)
{

}

VariableType::VariableType(const Type* type, bool is_const) : type(type), is_const(is_const)
{

}

const Type* VariableType::base() const { return type; }

const Type* VariableType::unqualified() const
{
    if ( type -> isReference() )
        return static_cast<const ReferenceType*>(type) -> type;
    else
        return type;
}
    
int VariableType::sizeOf() const { return type -> sizeOf(); } 

bool VariableType::isReference() const { return type -> isReference(); }
bool VariableType::isConst() const { return is_const; }
    
std::string VariableType::getName() const
{
    std::string res = "";
    res += isConst() ? "const~" : "";
    res += type -> getName();
    return res;
}
    
bool VariableType::operator==(const VariableType& vt) const { return type == vt.type && is_const == vt.is_const; } 
bool VariableType::operator!=(const VariableType& vt) const { return !(*this == vt); }
