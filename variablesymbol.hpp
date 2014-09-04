#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "type.hpp"

enum class VariableSymbolType { SIMPLE, PARAM, FIELD };

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(std::string name, const Type *type, VariableSymbolType sym_type = VariableSymbolType::SIMPLE);

    virtual const Type* getType() const;

    void setType(const Type *t);

    virtual std::string getName() const;

    bool isParam() const;
    bool isField() const;

    virtual SymbolType getSymbolType() const;
    
private:

	std::string name;
    const Type *type;

    VariableSymbolType sym_type;
};

#endif
