#include "symbol.hpp"

Symbol::~Symbol() { } 
    
bool Symbol::isVariable() const { return false; } 
bool Symbol::isType() const { return false; }
bool Symbol::isFunction() const { return false; }
bool Symbol::isModule() const { return false; }
