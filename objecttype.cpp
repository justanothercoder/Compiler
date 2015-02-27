#include "objecttype.hpp"
#include "typefactory.hpp"
#include "symbol.hpp"
#include "varsymbol.hpp"
#include "functionalsymbol.hpp"

#include "logger.hpp"

bool ObjectType::isObjectType() const { return true; }

bool ObjectType::hasConversionConstructor(const ObjectType *st) const 
{
    return getConversionConstructor(st) != nullptr; 
}

bool ObjectType::hasConversionOperator(const ObjectType *st) const 
{
    return getConversionOperator(st) != nullptr; 
}

const FunctionalSymbol* ObjectType::getConversionConstructor(const ObjectType *st) const
{
    auto ref_this     = VariableType(TypeFactory::getReference(this), false);
    auto ref_st       = VariableType(TypeFactory::getReference(st), false);
    auto const_ref_st = VariableType(TypeFactory::getReference(st), true);

    auto constr_const_ref = constructorWith({ref_this, const_ref_st});

    if ( constr_const_ref )
        return constr_const_ref;

    auto constr_ref = constructorWith({ref_this, ref_st});

    return constr_ref ? constr_ref : constructorWith({ref_this, st});
}

const FunctionalSymbol* ObjectType::getConversionOperator(const ObjectType *st) const
{
    return methodWith("operator " + st -> typeName(), {TypeFactory::getReference(this)});
}

const FunctionalSymbol* ObjectType::getCopyConstructor() const
{
    auto ref_this       = VariableType(TypeFactory::getReference(this), false);
    auto const_ref_this = VariableType(TypeFactory::getReference(this), true);

    return constructorWith({ref_this, const_ref_this});
}

const FunctionalSymbol* ObjectType::methodWith(const std::string& name, FunctionTypeInfo ft) const
{
    for ( const auto& meth : methods() )
    {
        if ( meth -> getName() == name && meth -> type().typeInfo() == ft )
            return meth;
    }
    return nullptr;
}

const FunctionalSymbol* ObjectType::getDefaultConstructor() const { return constructorWith({TypeFactory::getReference(this)}); }
const FunctionalSymbol* ObjectType::constructorWith(FunctionTypeInfo ft) const { return methodWith(typeName(), ft); }

int ObjectType::offsetOf(const std::string& name) const
{
    int offset = 0;

    for ( const auto& mem : members() )
    {
        if ( mem -> getName() == name )
            return offset;
    
        offset += mem -> typeOf().sizeOf();
    }

    throw std::logic_error("Not found");
}
