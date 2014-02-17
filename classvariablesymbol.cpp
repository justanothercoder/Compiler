#include "classvariablesymbol.hpp"

ClassVariableSymbol::ClassVariableSymbol(StructSymbol *sym) : sym(sym) { }

string ClassVariableSymbol::getName() const { return sym->getName(); }
int ClassVariableSymbol::getSize() const { return sym->getSize(); }

TypeKind ClassVariableSymbol::getTypeKind() const { return TypeKind::CLASSTYPE; }
