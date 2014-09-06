#include "classvariablesymbol.hpp"
#include "structsymbol.hpp"

ClassVariableSymbol::ClassVariableSymbol(StructSymbol *type) : VariableSymbol(type -> getName(), type) 
{

}

SymbolType ClassVariableSymbol::getSymbolType() const 
{
   	return SymbolType::CLASSVARIABLE; 
}
