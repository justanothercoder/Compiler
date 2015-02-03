#include "variablesymbol.hpp"
#include "variablesymboldefine.hpp"
#include "type.hpp"

VariableSymbol::VariableSymbol(const std::string& name, VariableType type, VariableSymbolType sym_type) : name(name)
                                                                                                        , type(type)
                                                                                                        , sym_type(sym_type)
{

}

VariableType VariableSymbol::getType() const { return type; }
std::string VariableSymbol::getName()  const { return name; }

bool VariableSymbol::isVariable() const { return true; }

std::unique_ptr<DefineSymbolVisitor> VariableSymbol::defineSymbolVisitor() const { return std::make_unique<VariableSymbolDefine>(); }

bool VariableSymbol::isParam() const { return sym_type == VariableSymbolType::PARAM; }
bool VariableSymbol::isField() const { return sym_type == VariableSymbolType::FIELD; }
