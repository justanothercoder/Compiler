#include "symbol.hpp"
#include "symboldefine.hpp"

Symbol::~Symbol() { } 
std::unique_ptr<DefineSymbolVisitor> Symbol::defineSymbolVisitor() const { return std::make_unique<SymbolDefine>(); }
    
bool Symbol::isVariable() const { return false; } 
bool Symbol::isType() const { return false; }
bool Symbol::isFunction() const { return false; }
bool Symbol::isModule() const { return false; }
