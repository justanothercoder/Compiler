#include "variablesymbol.hpp"
#include "type.hpp"

VariableSymbol::VariableSymbol(const std::string& name, VariableType type, VariableSymbolType sym_type) : name(name)
                                                                                                        , type(type)
                                                                                                        , sym_type(sym_type)
{

}

VariableType VariableSymbol::typeOf() const { return type; }
std::string VariableSymbol::getName()  const { return name; }

bool VariableSymbol::isVariable() const { return true; }

bool VariableSymbol::isParam() const { return sym_type == VariableSymbolType::PARAM; }
bool VariableSymbol::isField() const { return sym_type == VariableSymbolType::FIELD; }
