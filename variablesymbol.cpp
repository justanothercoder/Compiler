#include "variablesymbol.hpp"
#include "variablesymboldefine.hpp"
#include "type.hpp"

VariableSymbol::VariableSymbol(std::string name, const Type *type, VariableSymbolType sym_type) : name(name)
																						   , type(type)
																						   , sym_type(sym_type) 
{

}

const Type* VariableSymbol::getType() const 
{
   	return type; 
}

void VariableSymbol::setType(const Type *t) 
{
   	type = t; 
}

std::string VariableSymbol::getName() const 
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
	
ScopeVisitor& VariableSymbol::getScopeVisitor() 
{
	return *(new VariableSymbolDefine(this));
}
