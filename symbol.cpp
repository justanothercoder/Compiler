#include "symbol.hpp"
#include "symboldefine.hpp"

Symbol::~Symbol() { } 
std::unique_ptr<DefineSymbolVisitor> Symbol::defineSymbolVisitor() const { return std::make_unique<SymbolDefine>(); }
