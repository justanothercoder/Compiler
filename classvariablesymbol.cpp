#include "classvariablesymbol.hpp"

ClassVariableSymbol::ClassVariableSymbol(StructSymbol *type) : VariableSymbol(type->getName(), VariableType(type)) { }
