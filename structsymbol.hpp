#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "objecttype.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "structscope.hpp"
#include "globaltable.hpp"

class FunctionType;

class StructSymbol : public Symbol, public StructScope, public ObjectType
{
public:

    StructSymbol(std::string name, Scope *enclosing_scope, const TemplateInfo& template_info);

    SymbolType getSymbolType() const override;
    std::string getName() const override;

    size_t sizeOf() const override;
    TypeKind getTypeKind() const override;

    FunctionSymbol* getConversionTo(const Type *type) const;

    bool isConvertableTo(const Type *type) const override;

    boost::optional<int> rankOfConversion(const Type *type) const override;

    FunctionSymbol* getConversionConstructor(const StructSymbol *st) const;
    bool hasConversionConstructor(const StructSymbol *st) const;

    FunctionSymbol* getConversionOperator(const StructSymbol *st) const;
    bool hasConversionOperator(const StructSymbol *st) const;

    FunctionSymbol* getCopyConstructor() const;
    FunctionSymbol* getDefaultConstructor() const;

    FunctionSymbol* constructorWith(FunctionTypeInfo ft) const;

    FunctionSymbol* methodWith(std::string name, FunctionTypeInfo ft) const;

    bool isUnsafeBlock() const override;

    void defineBuiltInMethod(std::string name, FunctionType type);
    void defineBuiltInOperator(std::string name, FunctionType type);
    void defineBuiltInConstructor(FunctionType type);

    Symbol* resolveMember(std::string name) const override;
    FunctionalType* resolveMethod(std::string name) const override;

private:

    std::string name;
};

#endif
