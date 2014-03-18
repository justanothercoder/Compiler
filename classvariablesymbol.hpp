#ifndef _CLASSVARIABLESYMBOL_HPP_
#define _CLASSVARIABLESYMBOL_HPP_

#include "variablesymbol.hpp"
#include "structsymbol.hpp"

class ClassVariableSymbol : public VariableSymbol
{
public:

    ClassVariableSymbol(StructSymbol *type);

	StructSymbol *type;
};

#endif
