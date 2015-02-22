#ifndef _VARSYMBOL_HPP_
#define _VARSYMBOL_HPP_

#include "symbol.hpp"
#include "variabletype.hpp"

enum class VariableSymbolType { SIMPLE, PARAM, FIELD };

class VarSymbol : public Symbol
{
public:

    virtual bool isParam() const = 0;
    virtual bool isField() const = 0;

    virtual VariableType typeOf() const = 0;
};

#endif
