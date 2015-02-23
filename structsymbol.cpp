#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"
#include "comp.hpp"
#include "globalconfig.hpp"
#include "varsymbol.hpp"

#include "logger.hpp"

StructSymbol::StructSymbol(std::string name, StructScope* scope) : name(name), scope(scope) { }

bool StructSymbol::isConvertableTo(const Type* type) const
{
    type = type -> removeRef();

    if ( !type -> isObjectType() )
        return false;

    auto st = static_cast<const ObjectType*>(type);

    return static_cast<const ObjectType*>(this) == st || this -> hasConversionOperator(st) || st -> hasConversionConstructor(this);
}

const FunctionalSymbol* StructSymbol::getConversionTo(const Type* type) const
{
    type = type -> removeRef();

    auto st = static_cast<const StructSymbol*>(type);

    auto conv_operator = getConversionOperator(st);

    if ( conv_operator != nullptr )
        return conv_operator;

    auto conv_constr = st -> getConversionConstructor(this);

    return conv_constr;
}

boost::optional<int> StructSymbol::rankOfConversion(const Type *type) const
{
    if ( !isConvertableTo(type) )
        return boost::none;

    type = type -> removeRef();

    auto st = static_cast<const StructSymbol*>(type);

    return (this == st) ? 0 : 3;
}

size_t StructSymbol::sizeOf() const { return scope -> type_size; } 
std::string StructSymbol::getName() const { return name; }
std::string StructSymbol::typeName() const { return name; }

bool StructSymbol::isType() const { return true; }

Scope* StructSymbol::innerScope() const { return scope; }
    
std::vector<VarSymbol*> StructSymbol::members()        const { return scope -> getVars(); }
std::vector<FunctionalSymbol*> StructSymbol::methods() const { return scope -> getFunctions(); }
    
void StructSymbol::defineMethod(std::unique_ptr<FunctionalSymbol> method) { scope -> define(std::move(method)); }
void StructSymbol::defineMember(std::unique_ptr<VarSymbol> member)        { scope -> define(std::move(member)); }
