#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "variabletype.hpp"

enum class VariableSymbolType { SIMPLE, PARAM, FIELD };

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(const std::string& name, VariableType type, VariableSymbolType sym_type = VariableSymbolType::SIMPLE);

    VariableType getType() const;
    std::string getName() const override;

    bool isParam() const;
    bool isField() const;

    SymbolType getSymbolType() const override;

    std::unique_ptr<DefineSymbolVisitor> defineSymbolVisitor() const override;

private:

    std::string name;
    VariableType type;

    VariableSymbolType sym_type;
};

#endif
