#ifndef _BUILTINTYPESYMBOL_HPP_
#define _BUILTINTYPESYMBOL_HPP_

#include "type.hpp"
#include "symbol.hpp"

class BuiltInTypeSymbol : public Symbol, public Type
{
public:

    BuiltInTypeSymbol(string name, int size);
    
	std::string getName() const override;
    size_t getSize() const override;

    SymbolType getSymbolType() const override;

    TypeKind getTypeKind() const override;

	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *type) const override;
	
	FunctionSymbol* getConversionTo(const Type *type) const override;

private:

	std::string name;
    size_t size;

};

#endif
