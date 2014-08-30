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
	
bool BuiltInTypeSymbol::isConvertableTo(const Type *type) const
{
	return this == type;
}
	
boost::optional<int> BuiltInTypeSymbol::rankOfConversion(const Type *type) const 
{
	return isConvertableTo(type) ? 0 : boost::none;
}
	
bool BuiltInTypeSymbol::isReference() const 
{
	return false;
}

bool BuiltInTypeSymbol::isConst() const 
{
	return false;
}

FunctionSymbol* BuiltInTypeSymbol::getConversionTo(const Type *) const 
{
	return nullptr;
}
