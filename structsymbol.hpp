#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "scope.hpp"
#include "symbol.hpp"
#include "objecttype.hpp"
#include "structscope.hpp"
#include "functiontype.hpp"

class VariableSymbol;

class StructSymbol : public Symbol, public StructScope, public ObjectType
{
public:

    StructSymbol(const std::string& name, Scope* enclosing_scope, const TemplateInfo& template_info);

    SymbolType getSymbolType() const override;
    std::string getName() const override;

    size_t sizeOf() const override;
    TypeKind getTypeKind() const override;

    const FunctionSymbol* getConversionTo(const Type *type) const;
    bool isConvertableTo(const Type *type) const override;

    boost::optional<int> rankOfConversion(const Type *type) const override;

    bool hasConversionConstructor(const StructSymbol *st) const;
    bool hasConversionOperator(const StructSymbol *st) const;

    const FunctionSymbol* getConversionConstructor(const StructSymbol *st) const;
    const FunctionSymbol* getConversionOperator(const StructSymbol *st) const;

    const FunctionSymbol* getCopyConstructor() const;
    const FunctionSymbol* getDefaultConstructor() const;

    const FunctionSymbol* methodWith(const std::string& name, FunctionTypeInfo ft) const;
    const FunctionSymbol* constructorWith(FunctionTypeInfo ft) const;

    bool isUnsafeBlock() const override;

    void defineBuiltInMethod(std::string name, FunctionType type);
    void defineBuiltInOperator(std::string name, FunctionType type);
    void defineBuiltInConstructor(FunctionType type);

    const Symbol* resolveMember(const std::string& name) const override;
    const FunctionalType* resolveMethod(const std::string& name) const override;
    int offsetOf(const VariableSymbol* member) const override;

private:

    std::string name;
};

#endif
