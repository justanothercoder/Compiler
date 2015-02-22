#ifndef _SYMBOLENTRY_HPP_
#define _SYMBOLENTRY_HPP_

#include "symbol.hpp"

enum class SymbolType { Functional, Type, Var, BuiltIn, Module, Alias, Template };

class SymbolEntry
{
public:
    
    SymbolEntry(std::unique_ptr<Symbol> symbol, SymbolType sym_type);    

    SymbolType type() const;
    Symbol* symbol() const;

private:

    SymbolType sym_type;
    std::unique_ptr<Symbol> symbol_;
};

#endif
