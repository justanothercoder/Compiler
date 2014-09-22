#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(std::string name, int size) : name(name), size(size) 
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
	
FunctionSymbol* BuiltInTypeSymbol::getConversionTo(const Type *) const 
{
	return nullptr;
}
	
const Symbol* BuiltInTypeSymbol::getSymbol() const
{
	return this;
}
	
void BuiltInTypeSymbol::accept(TypeVisitor& visitor) const 
{
	visitor.visit(this);
}
