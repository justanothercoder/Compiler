#include "structsymbol.hpp"

#include "callhelper.hpp"
#include "functionsymbol.hpp"

StructSymbol::StructSymbol(string name, Scope *enclosing_scope) : StructScope(name, enclosing_scope)
																, name(name)
{

//	scope_name = getEnclosingScope() -> getScopeName() + "_" + name;
}

int StructSymbol::getSize() const 
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

bool StructSymbol::isConvertableTo(StructSymbol *st)
{
	return this == st || this -> hasConversionOperator(st) || st -> hasConversionConstructor(this);
}
	
bool StructSymbol::hasConversionConstructor(StructSymbol *st)
{
	return getConversionConstructor(st) != nullptr;
}

bool StructSymbol::hasConversionOperator(StructSymbol *st)
{
	return getConversionOperator(st) != nullptr;
}

FunctionSymbol* StructSymbol::getConversionConstructor(StructSymbol *st)
{
	auto constr_const_ref = constructorWith({VariableType(this, true), VariableType(st, true, true)});

	if ( constr_const_ref )
		return constr_const_ref;

	auto constr_ref = constructorWith({VariableType(this, true), VariableType(st, true)});
	
	return constr_ref ? constr_ref : constructorWith({VariableType(this, true), VariableType(st)});
}

FunctionSymbol* StructSymbol::getConversionOperator(StructSymbol *st)
{
	string cast_operator_name = "operator " + st -> getName();

	auto func_sym = resolveMember(cast_operator_name);

	if ( func_sym == nullptr )
		return nullptr;

	auto conv_oper = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(func_sym) -> getType().type);

	auto info = conv_oper -> getTypeInfo();

	auto it = info.symbols.find({ });

	return it == std::end(info.symbols) ? nullptr : it -> second;
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
	
FunctionSymbol* StructSymbol::constructorWith(FunctionTypeInfo ft)
{
/*
	string constructor_name = getName();

	auto member = dynamic_cast<VariableSymbol*>(resolveMember(constructor_name));

	if ( member == nullptr )
		return nullptr;

	auto func = dynamic_cast<OverloadedFunctionSymbol*>(member -> getType().type);
	auto info = func -> getTypeInfo();

	auto it = info.symbols.find(ft);
	return it == std::end(info.symbols) ? nullptr : it -> second;
*/
	return methodWith(getName(), ft);
}
	
int StructSymbol::rankOfConversion(StructSymbol *st)
{
	return (this == st) ? 0 : 1;
}
	
FunctionSymbol* StructSymbol::methodWith(string name, FunctionTypeInfo ft)
{
	auto member = dynamic_cast<VariableSymbol*>(resolveMember(name));
	if ( member == nullptr )
		return nullptr;

	auto func = dynamic_cast<OverloadedFunctionSymbol*>(member -> getType().type);
	auto info = func -> getTypeInfo();

	auto it = info.symbols.find(ft);
	return it == std::end(info.symbols) ? nullptr : it -> second;
}
