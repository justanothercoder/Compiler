#include "structsymbol.hpp"

#include "callhelper.hpp"
#include "functionsymbol.hpp"

StructSymbol::StructSymbol(string name
		                 , Scope *enclosing_scope
						 , const TemplateInfo& template_info) : StructScope(name, enclosing_scope, template_info)
															  , name(name)
{

}

size_t StructSymbol::getSize() const 
{ 
	return type_size; 
}

string StructSymbol::getName() const 
{ 
	return name; 
}

SymbolType StructSymbol::getSymbolType() const 
{ 
	return SymbolType::STRUCT; 
}

TypeKind StructSymbol::getTypeKind() const
{
   	return TypeKind::STRUCT; 
}

bool StructSymbol::isConvertableTo(const Type *type) const
{
	if ( type -> getTypeKind() != this -> getTypeKind() )
		return false;

	auto st = static_cast<const StructSymbol*>(type);

	return this == st || this -> hasConversionOperator(st) || st -> hasConversionConstructor(this);
}
	
bool StructSymbol::hasConversionConstructor(const StructSymbol *st) const
{
	return getConversionConstructor(st) != nullptr;
}

bool StructSymbol::hasConversionOperator(const StructSymbol *st) const
{
	return getConversionOperator(st) != nullptr;
}

FunctionSymbol* StructSymbol::getConversionConstructor(const StructSymbol *st) const
{
	auto constr_const_ref = constructorWith({new VariableType(this, true), new VariableType(st, true, true)});

	if ( constr_const_ref )
		return constr_const_ref;

	auto constr_ref = constructorWith({new VariableType(this, true), new VariableType(st, true)});
	
	return constr_ref ? constr_ref : constructorWith({new VariableType(this, true), new VariableType(st)});
}

FunctionSymbol* StructSymbol::getConversionOperator(const StructSymbol *st) const
{
	return methodWith("operator " + st -> getName(), {new VariableType(this, true)});
}

FunctionSymbol* StructSymbol::getConversionTo(StructSymbol *st)
{
	auto conv_operator = getConversionOperator(st);

	if ( conv_operator != nullptr )
		return conv_operator;

	auto conv_constr = st -> getConversionConstructor(this);

	return conv_constr;
}
	
FunctionSymbol* StructSymbol::getCopyConstructor()
{
	return constructorWith({VariableType(this, true), VariableType(this, true, true)});
}

FunctionSymbol* StructSymbol::getDefaultConstructor()
{
	return constructorWith({VariableType(this, true)});
}
	
FunctionSymbol* StructSymbol::constructorWith(FunctionTypeInfo ft) const
{
	return methodWith(getName(), ft);
}
	
boost::optional<int> StructSymbol::rankOfConversion(Type *type) const
{
	if ( !isConvertableTo(type) )
		return boost::none;

	StructSymbol *st = static_cast<StructSymbol*>(type);

	return (this == st) ? 0 : 1;
}
	
FunctionSymbol* StructSymbol::methodWith(string name, FunctionTypeInfo ft) const
{
	auto member = dynamic_cast<VariableSymbol*>(resolveMember(name));
	if ( member == nullptr )
		return nullptr;

	auto func = dynamic_cast<const OverloadedFunctionSymbol*>(member -> getType().type);
	auto info = func -> getTypeInfo();

	auto it = info.symbols.find(ft);
	return it == std::end(info.symbols) ? nullptr : it -> second;
}
