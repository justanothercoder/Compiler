#include "aliassymbol.hpp"

AliasSymbol::AliasSymbol(Symbol* aliased) : aliased(aliased) { }

std::string AliasSymbol::getName() const { return aliased -> getName(); }

bool AliasSymbol::isType() const     { return aliased -> isType(); }
bool AliasSymbol::isModule() const   { return aliased -> isModule(); }
bool AliasSymbol::isVariable() const { return aliased -> isVariable(); }
bool AliasSymbol::isFunction() const { return aliased -> isFunction(); }
    
Symbol* AliasSymbol::symbol() const { return aliased; }
