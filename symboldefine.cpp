#include "symboldefine.hpp"

SymbolDefine::SymbolDefine(Symbol *sym) : sym(sym) { }

void SymbolDefine::visit(GlobalScope *sc) { sc->table[sym->getName()] = sym; }
void SymbolDefine::visit(LocalScope *sc) { sc->table[sym->getName()] = sym; }
void SymbolDefine::visit(StructSymbol *sc) { sc->table[sym->getName()] = sym; }
void SymbolDefine::visit(FunctionSymbol *sc) { sc->table[sym->getName()] = sym; }
