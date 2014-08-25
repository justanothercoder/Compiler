#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_ #include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "fieldvarallocator.hpp"

#include "structscope.hpp"

using std::map;

class StructSymbol : public Symbol, public StructScope, public Type
{
public:

    StructSymbol(string name, Scope *enclosing_scope, const TemplateInfo& template_info);

    SymbolType getSymbolType() const override;
    string getName() const override;

    size_t getSize() const override;
    TypeKind getTypeKind() const override;

	FunctionSymbol* getConversionTo(StructSymbol *st);

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

	bool isReference() const override;
	bool isConst() const override;

private:

    string name;
};
		     
#endif
