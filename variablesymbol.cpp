#include "variablesymbol.hpp"

VariableSymbol::VariableSymbol(string name, Type *type, VariableSymbolType sym_type) : name(name), type(type), sym_type(sym_type) { }

Type* VariableSymbol::getType() const { return type; }
void VariableSymbol::setType(Type *t) { type = t; }

string VariableSymbol::getName() const { return name; }

bool VariableSymbol::isParam() const { return sym_type == VariableSymbolType::PARAM; }
bool VariableSymbol::isField() const { return sym_type == VariableSymbolType::FIELD; }
