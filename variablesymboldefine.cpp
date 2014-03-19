#include "variablesymboldefine.hpp"

VariableSymbolDefine::VariableSymbolDefine(VariableSymbol *sym) : sym(sym) { }

void VariableSymbolDefine::visit(GlobalScope *sc)
{
	sc->table[sym->getName()] = sym;
	sc->get_valloc()->addVariable(sym);
}

void VariableSymbolDefine::visit(LocalScope *sc)
{
	sc->table[sym->getName()] = sym;
	sc->get_valloc()->addVariable(sym);
}

void VariableSymbolDefine::visit(StructSymbol *sc)
{
	sc->table[sym->getName()] = sym;
	sc->get_valloc()->addVariable(sym);
	sc->type_size += sym->getType().getSize();
}

void VariableSymbolDefine::visit(FunctionSymbol *sc)
{
	sc->table[sym->getName()] = sym;
	sc->get_valloc()->addVariable(sym);
}
