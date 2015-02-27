#include "variabletype.hpp"
#include "type.hpp"
#include "referencetype.hpp"
#include "pointertype.hpp"
#include "arraytype.hpp"
#include "typeinfo.hpp"
#include "numbernode.hpp"

#include "logger.hpp"

VariableType::VariableType(const Type* type) : VariableType(type, false) { } 
VariableType::VariableType(const Type* type, bool is_const) : type(type), is_const(is_const) { } 

const Type* VariableType::base() const { return type; }
const Type* VariableType::unqualified() const { return type -> removeRef(); }
    
int VariableType::sizeOf() const { return type -> sizeOf(); } 

bool VariableType::isReference() const { return type -> isReference(); }
bool VariableType::isConst() const { return is_const; }
    
std::string VariableType::getName() const
{
    std::string res = "";
    res += isConst() ? "const~" : "";
    res += type -> typeName();
    return res;
}
    
bool VariableType::operator==(const VariableType& vt) const { return type == vt.type && is_const == vt.is_const; } 
bool VariableType::operator!=(const VariableType& vt) const { return !(*this == vt); }

TypeInfo makeTypeInfo(VariableType type) 
{
    auto tp = type.unqualified();

    auto modifiers = std::vector<TypeModifier>{ };

    while ( tp -> isPointer() || tp -> isArray() )
    {
        if ( tp -> isPointer() )
        {
            modifiers.emplace_back();
            tp = static_cast<const PointerType*>(tp) -> pointedType();
        }
        else
        {
            auto arr = static_cast<const ArrayType*>(tp);
            modifiers.emplace_back(std::make_shared<NumberNode>(std::to_string(arr -> sizeOfArray())));
            tp = arr -> pointedType();
        }
    }

    return TypeInfo(type.unqualified() -> typeName(), type.isReference(), type.isConst(), { }, modifiers);
}
