#include "variablesymbol.hpp"

VariableSymbol::VariableSymbol(string name, VariableType type, VariableSymbolType sym_type) : name(name)
																							, type(type)
																							, sym_type(sym_type) 
{

}

VariableType VariableSymbol::getType() const 
{
   	return type; 
}

void VariableSymbol::setType(VariableType t) 
{
   	type = t; 
}

string VariableSymbol::getName() const 
{
   	return name; 
}

bool VariableSymbol::isParam() const 
{
   	return sym_type == VariableSymbolType::PARAM; 
}

bool VariableSymbol::isField() const 
{
	return sym_type == VariableSymbolType::FIELD; 
}

SymbolType VariableSymbol::getSymbolType() const 
{
   	return SymbolType::VARIABLE; 
}
