#include "variablesymboldefine.hpp"

VariableSymbolDefine::VariableSymbolDefine(VariableSymbol *sym) : sym(sym) { }

void VariableSymbolDefine::visit(GlobalScope *sc)
{
	sc->table[sym->getName()] = sym;
	sc->addresses[sym] = sc->scope_size + GlobalConfig::int_size;
	sc->scope_size += sym->getType()->getSize();

	sc->increaseScopeTreeSize(sym->getType()->getSize());
}

void VariableSymbolDefine::visit(LocalScope *sc)
{
	sc->table[sym->getName()] = sym;
	sc->addresses[sym] = sc->scope_size + GlobalConfig::int_size;
	sc->scope_size += sym->getType()->getSize();

	sc->increaseScopeTreeSize(sym->getType()->getSize());
}

void VariableSymbolDefine::visit(StructSymbol *sc)
{
	sc->table[sym->getName()] = sym;
	sc->addresses[sym] = sc->scope_size;
	sc->scope_size += sym->getType()->getSize();
	sc->type_size += sym->getType()->getSize();
}

void VariableSymbolDefine::visit(FunctionSymbol *sc)
{
	sc->table[sym->getName()] = sym;

	if ( sym->isParam() )
	{
		sc->params_size += sym->getType()->getSize();
		sc->addresses[sym] = -sc->params_size;
	}
	else
	{
		sc->addresses[sym] = sc->scope_size + GlobalConfig::int_size;
		sc->scope_size += sym->getType()->getSize();
	}

	sc->increaseScopeTreeSize(sym->getType()->getSize());
}
