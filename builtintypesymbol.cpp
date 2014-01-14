#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(string name, int size) : name(name), size(size) { }
    
string BuiltInTypeSymbol::getName() const { return name; }

int BuiltInTypeSymbol::getSize() const { return size; }

