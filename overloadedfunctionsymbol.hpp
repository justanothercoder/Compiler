#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "type.hpp"
#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "functiontraits.hpp"

class OverloadedFunctionSymbol : public Symbol, public Type
{
public:

    OverloadedFunctionSymbol(std::string name, OverloadedFunctionTypeInfo type_info, FunctionTraits traits);
	
	void accept(TypeVisitor& visitor) const override;

	std::string getName() const override;
    size_t getSize() const override;

    OverloadedFunctionTypeInfo getTypeInfo() const;

	void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym) const;

    bool isMethod() const;
    bool isConstructor() const;
    bool isOperator() const;

    const Type* getBaseType() const;

    SymbolType getSymbolType() const override;
    TypeKind getTypeKind() const override;
    
	FunctionSymbol* getViableOverload(FunctionTypeInfo params_type) const;

	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *type) const override;
	
	FunctionSymbol* getConversionTo(const Type *type) const override;

	const Symbol* getSymbol() const override;
    
private:

	std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
};

#endif
