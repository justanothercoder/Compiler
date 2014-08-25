#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(string name, int size) : name(name), size(size) 
{

}
    
std::string BuiltInTypeSymbol::getName() const 
{
   	return name; 
}

size_t BuiltInTypeSymbol::getSize() const 
{
   	return size; 
}

SymbolType BuiltInTypeSymbol::getSymbolType() const 
{
   	return SymbolType::BUILTINTYPE; 
}

TypeKind BuiltInTypeSymbol::getTypeKind() const 
{ 
	return TypeKind::BUILTIN; 
}
	
bool BuiltInTypeSymbol::isConvertableTo(Type *type) const
{
	return this == type;
}
