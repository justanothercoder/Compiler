#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "variabletype.hpp"

enum class VariableSymbolType { SIMPLE, PARAM, FIELD };

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(string name, VariableType type, VariableSymbolType sym_type = VariableSymbolType::SIMPLE);

    virtual VariableType getType() const;

    void setType(VariableType t);

    virtual string getName() const;

    bool isParam() const;
    bool isField() const;

    virtual SymbolType getSymbolType() const;
    
private:

    string name;
    VariableType type;

    VariableSymbolType sym_type;
};

#endif
