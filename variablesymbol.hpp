#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "variabletype.hpp"

#include "varsymbol.hpp"

class VariableSymbol : public VarSymbol
{
public:

    VariableSymbol(std::string name, VariableType type, VariableSymbolType sym_type = VariableSymbolType::SIMPLE);

    bool isParam() const override;
    bool isField() const override;

    VariableType typeOf() const;
    std::string getName() const override;

    bool isVariable() const override;

private:

    std::string name;
    VariableType type;

    VariableSymbolType sym_type;
};

#endif
