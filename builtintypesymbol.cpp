#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(const std::string& name, int size) : name(name), size(size) { }

std::string BuiltInTypeSymbol::getName() const { return name; } 
size_t BuiltInTypeSymbol::sizeOf() const { return size; }

bool BuiltInTypeSymbol::isConvertableTo(const Type *type) const { return this == type; }
boost::optional<int> BuiltInTypeSymbol::rankOfConversion(const Type *type) const { return isConvertableTo(type) ? 0 : boost::none; }
const FunctionSymbol* BuiltInTypeSymbol::getConversionTo(const Type *) const { return nullptr; }
