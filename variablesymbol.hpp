#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "type.hpp"

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(string name, Type *type);

private:

    Type *type;
};

#endif
