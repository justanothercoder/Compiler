#ifndef _BUILTINTYPESYMBOL_HPP_
#define _BUILTINTYPESYMBOL_HPP_

#include "type.hpp"
#include "symbol.hpp"

class BuiltInTypeSymbol : public Symbol, public Type
{
public:

    BuiltInTypeSymbol(const std::string& name, int size);

    std::string getName() const override;

    std::string typeName() const override; 
    size_t sizeOf() const override;

    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *type) const override;

    const FunctionalSymbol* getConversionTo(const Type *type) const override;

    bool isType() const override;

private:

    std::string name;
    size_t size;

};

#endif
