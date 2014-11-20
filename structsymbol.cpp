#include "structsymbol.hpp"

#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"

StructSymbol::StructSymbol(std::string name
                           , Scope *enclosing_scope
                           , const TemplateInfo& template_info) : StructScope(name, enclosing_scope, template_info)
    , name(name)
{

}

size_t StructSymbol::getSize() const
{
    return type_size;
}

std::string StructSymbol::getName() const
{
    return name;
}

SymbolType StructSymbol::getSymbolType() const
{
    return SymbolType::STRUCT;
}

TypeKind StructSymbol::getTypeKind() const
{
    return TypeKind::STRUCT;
}

bool StructSymbol::isConvertableTo(const Type *type) const
{
    type = type -> getUnqualifiedType();

    if ( type -> getTypeKind() != this -> getTypeKind() )
        return false;

    auto st = static_cast<const StructSymbol*>(type);

    return this == st || this -> hasConversionOperator(st) || st -> hasConversionConstructor(this);
}

bool StructSymbol::hasConversionConstructor(const StructSymbol *st) const
{
    return getConversionConstructor(st) != nullptr;
}

bool StructSymbol::hasConversionOperator(const StructSymbol *st) const
{
    return getConversionOperator(st) != nullptr;
}

FunctionSymbol* StructSymbol::getConversionConstructor(const StructSymbol *st) const
{
    auto ref_this     = TypeFactory::getReference(this);
    auto ref_st       = TypeFactory::getReference(st);
    auto const_ref_st = TypeFactory::getConst(ref_st);

    auto constr_const_ref = constructorWith({ref_this, const_ref_st});

    if ( constr_const_ref )
        return constr_const_ref;

    auto constr_ref = constructorWith({ref_this, ref_st});

    return constr_ref ? constr_ref : constructorWith({ref_this, st});
}

FunctionSymbol* StructSymbol::getConversionOperator(const StructSymbol *st) const
{
    return methodWith("operator " + st -> getName(), {TypeFactory::getReference(this)});
}

FunctionSymbol* StructSymbol::getConversionTo(const Type *type) const
{
//	if ( type -> getTypeKind() != TypeKind::STRUCT )
//		return nullptr;

    type = type -> getUnqualifiedType();

    auto st = static_cast<const StructSymbol*>(type);

    auto conv_operator = getConversionOperator(st);

    if ( conv_operator != nullptr )
        return conv_operator;

    auto conv_constr = st -> getConversionConstructor(this);

    return conv_constr;
}

FunctionSymbol* StructSymbol::getCopyConstructor() const
{
    auto ref_this       = TypeFactory::getReference(this);
    auto const_ref_this = TypeFactory::getConst(ref_this);

    return constructorWith({ref_this, const_ref_this});
}

FunctionSymbol* StructSymbol::getDefaultConstructor() const
{
    return constructorWith({TypeFactory::getReference(this)});
}

FunctionSymbol* StructSymbol::constructorWith(FunctionTypeInfo ft) const
{
    return methodWith(getName(), ft);
}

boost::optional<int> StructSymbol::rankOfConversion(const Type *type) const
{
    if ( !isConvertableTo(type) )
        return boost::none;

    type = type -> getUnqualifiedType();

    auto st = static_cast<const StructSymbol*>(type);

    return (this == st) ? 0 : 3;
}

FunctionSymbol* StructSymbol::methodWith(std::string name, FunctionTypeInfo ft) const
{
    auto member = resolveMember(name);
    if ( member == nullptr || member -> getSymbolType() != SymbolType::OVERLOADED_FUNCTION )
        return nullptr;

    auto func = dynamic_cast<const OverloadedFunctionSymbol*>(member);
    auto info = func -> getTypeInfo();

    auto it = info.symbols.find(ft);
    return it == std::end(info.symbols) ? nullptr : it -> second;
}

const Symbol* StructSymbol::getSymbol() const
{
    return this;
}

bool StructSymbol::isUnsafeBlock() const
{
    return is_unsafe;
}
    
void StructSymbol::defineBuiltInMethod(std::string name, const FunctionType *type)
{
    define(new FunctionSymbol(name
                            , type
                            , new FunctionScope(getScopeName() + "_" + name
                                              , this
                                              , false
                                              , false)
                            , {true, false, false}
                            )
            );
}

void StructSymbol::defineBuiltInOperator(std::string name, const FunctionType *type)
{
    define(new FunctionSymbol(name
                            , type
                            , new FunctionScope(getScopeName() + "_" + GlobalConfig::getCodeOperatorName(name)
                                              , this
                                              , false
                                              , false)
                            , {true, false, true}
                            )
            );
}

void StructSymbol::defineBuiltInConstructor(const FunctionType *type)
{
    define(new FunctionSymbol(name
                            , type
                            , new FunctionScope(getScopeName() + "_" + name
                                              , this
                                              , true
                                              , false)
                            , {true, true, false}
                            )
            );
}
