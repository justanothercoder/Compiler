#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(const std::string& name, int size) : name(name), size(size) { }

std::string BuiltInTypeSymbol::getName() const { return name; } 
std::string BuiltInTypeSymbol::typeName() const { return name; } 
size_t BuiltInTypeSymbol::sizeOf() const { return size; }

bool BuiltInTypeSymbol::isConvertableTo(const Type *type) const { return this == type; }
boost::optional<int> BuiltInTypeSymbol::rankOfConversion(const Type *type) const { return isConvertableTo(type) ? 0 : boost::none; }
const FunctionalSymbol* BuiltInTypeSymbol::getConversionTo(const Type *) const { return nullptr; }
    
bool BuiltInTypeSymbol::isType() const { return true; }
