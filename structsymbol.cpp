#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"
#include "comp.hpp"
#include "globalconfig.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

StructSymbol::StructSymbol(const std::string& name
                         , Scope* enclosing_scope) : StructScope(name, enclosing_scope), name(name)
{

}

bool StructSymbol::isConvertableTo(const Type* type) const
{
    type = type -> removeRef();

    if ( type -> getTypeKind() != this -> getTypeKind() )
        return false;

    auto st = static_cast<const StructSymbol*>(type);

    return this == st || this -> hasConversionOperator(st) || st -> hasConversionConstructor(this);
}

const FunctionSymbol* StructSymbol::getConversionConstructor(const StructSymbol *st) const
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

const FunctionSymbol* StructSymbol::getConversionOperator(const StructSymbol *st) const
{
    return methodWith("operator " + st -> getName(), {TypeFactory::getReference(this)});
}

const FunctionSymbol* StructSymbol::getConversionTo(const Type* type) const
{
    type = type -> removeRef();

    auto st = static_cast<const StructSymbol*>(type);

    auto conv_operator = getConversionOperator(st);

    if ( conv_operator != nullptr )
        return conv_operator;

    auto conv_constr = st -> getConversionConstructor(this);

    return conv_constr;
}

const FunctionSymbol* StructSymbol::getCopyConstructor() const
{
    auto ref_this       = VariableType(TypeFactory::getReference(this), false);
    auto const_ref_this = VariableType(TypeFactory::getReference(this), true);

    return constructorWith({ref_this, const_ref_this});
}

boost::optional<int> StructSymbol::rankOfConversion(const Type *type) const
{
    if ( !isConvertableTo(type) )
        return boost::none;

    type = type -> removeRef();

    auto st = static_cast<const StructSymbol*>(type);

    return (this == st) ? 0 : 3;
}

const FunctionSymbol* StructSymbol::methodWith(const std::string& name, FunctionTypeInfo ft) const
{
    auto member = resolveMember(name);
    if ( member == nullptr || member -> getSymbolType() != SymbolType::OVERLOADED_FUNCTION )
        return nullptr;

    auto func = static_cast<const OverloadedFunctionSymbol*>(member);
    return func -> overloadWith(ft);
}

void StructSymbol::defineBuiltInMethod(std::string name, FunctionType type)
{
    auto scope_name = getScopeName() + "_" + name;
    auto scope = new FunctionScope(scope_name, this, false);
    define(std::make_shared<const FunctionSymbol>(name, type, scope, FunctionTraits::method()));
}

void StructSymbol::defineBuiltInOperator(std::string name, FunctionType type)
{
    auto scope_name = getScopeName() + "_" + Comp::config().getCodeOperatorName(name);
    auto scope = new FunctionScope(scope_name, this, false);
    define(std::make_shared<const FunctionSymbol>(name, type, scope, FunctionTraits::methodOper()));
}

void StructSymbol::defineBuiltInConstructor(FunctionType type)
{
    auto scope_name = getScopeName() + "_" + name;
    auto scope = new FunctionScope(scope_name, this, false);
    define(std::make_shared<const FunctionSymbol>(name, type, scope, FunctionTraits::constructor()));
}

SymbolType StructSymbol::getSymbolType() const { return SymbolType::STRUCT; }
TypeKind StructSymbol::getTypeKind() const { return TypeKind::STRUCT; }

size_t StructSymbol::sizeOf() const { return type_size; } 
std::string StructSymbol::getName() const { return name; }

bool StructSymbol::isUnsafeBlock() const { return is_unsafe; }
    
bool StructSymbol::hasConversionConstructor(const StructSymbol *st) const { return getConversionConstructor(st) != nullptr; }
bool StructSymbol::hasConversionOperator(const StructSymbol *st) const { return getConversionOperator(st) != nullptr; }

const FunctionSymbol* StructSymbol::getDefaultConstructor() const { return constructorWith({TypeFactory::getReference(this)}); }
const FunctionSymbol* StructSymbol::constructorWith(FunctionTypeInfo ft) const { return methodWith(getName(), ft); }

const Symbol* StructSymbol::resolveMember(const std::string& name) const { return resolveHere(name); }
const FunctionalType* StructSymbol::resolveMethod(const std::string& name) const { return static_cast<const OverloadedFunctionSymbol*>(resolveMember(name)); }

int StructSymbol::offsetOf(const VariableSymbol* member) const
{
    int offset = 0;

    for ( auto entry : table )
    {
        if ( entry.second -> getSymbolType() == SymbolType::VARIABLE )
        {
            if ( entry.second.get() == member )
                return offset;
            else
                offset += static_cast<const VariableSymbol*>(entry.second.get()) -> getType().sizeOf();
        }
    }

    throw std::logic_error("Not found");
}
