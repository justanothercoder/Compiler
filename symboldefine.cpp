#include "symboldefine.hpp"

SymbolDefine::SymbolDefine(Symbol *sym) : sym(sym) { }

void SymbolDefine::visit(GlobalScope *sc) 
{
	if ( sc -> table . find(sym -> getName()) != std::end(sc -> table) )
		throw SemanticError("Error: " + sym -> getName() + " is already defined");

	sc -> table[sym -> getName()] = sym; 	
}

void SymbolDefine::visit(LocalScope *sc) 
{
	if ( sc -> table . find(sym -> getName()) != std::end(sc -> table) )
		throw SemanticError("Error: " + sym -> getName() + " is already defined");

	sc -> table[sym -> getName()] = sym; 
}

void SymbolDefine::visit(StructSymbol *sc) 
{ 
	if ( sc -> table . find(sym -> getName()) != std::end(sc -> table) )
		throw SemanticError("Error: " + sym -> getName() + " is already defined");

	sc -> table[sym -> getName()] = sym; 
}

void SymbolDefine::visit(FunctionSymbol *sc) 
{ 
	if ( sc -> table . find(sym -> getName()) != std::end(sc -> table) )
		throw SemanticError("Error: " + sym -> getName() + " is already defined");

	sc -> table[sym -> getName()] = sym; 
}
