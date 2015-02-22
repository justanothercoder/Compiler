#include "symbolentry.hpp"

SymbolEntry::SymbolEntry(std::unique_ptr<Symbol> symbol, SymbolType sym_type) : sym_type(sym_type), symbol_(std::move(symbol)) { }

SymbolType SymbolEntry::type() const { return sym_type; }
Symbol* SymbolEntry::symbol() const { return symbol_.get(); }

