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

    int getSize() const override;
    TypeKind getTypeKind() const override;

	FunctionSymbol* getConversionTo(StructSymbol *st);
	bool isConvertableTo(StructSymbol *st);

	int rankOfConversion(StructSymbol *st);

	FunctionSymbol* getConversionConstructor(StructSymbol *st);
	bool hasConversionConstructor(StructSymbol *st);

	FunctionSymbol* getConversionOperator(StructSymbol *st);
	bool hasConversionOperator(StructSymbol *st);

	FunctionSymbol* getCopyConstructor();
	FunctionSymbol* getDefaultConstructor();

	FunctionSymbol* constructorWith(FunctionTypeInfo ft);

	FunctionSymbol* methodWith(string name, FunctionTypeInfo ft);

private:

    string name;
};
		     
#endif
