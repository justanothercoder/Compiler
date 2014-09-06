#ifndef _CLASSVARIABLESYMBOL_HPP_
#define _CLASSVARIABLESYMBOL_HPP_

#include "variablesymbol.hpp"

class StructSymbol;

class ClassVariableSymbol : public VariableSymbol
{
public:
    ClassVariableSymbol(StructSymbol *type);

	SymbolType getSymbolType() const override;
};

#endif
