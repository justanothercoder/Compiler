#include "structsymbol.hpp"

#include "callhelper.hpp"

StructSymbol::StructSymbol(string name, Scope *enclosing_scope) : name(name), enclosing_scope(enclosing_scope), valloc()
{
	type_size = 0;

	scope_name = getEnclosingScope()->getScopeName() + "_" + name;
}

Scope* StructSymbol::getEnclosingScope() const { return enclosing_scope; }

Symbol* StructSymbol::resolve(string name) const
{
	auto it = table.find(name);
	if ( it == std::end(table) )
	{
		if ( getEnclosingScope() )
			return getEnclosingScope()->resolve(name);
		return nullptr;
	}
	return it->second;
}

Symbol* StructSymbol::resolveMember(string name) const
{
	auto it = table.find(name);
	if ( it == std::end(table) )
		return nullptr;
	return it->second;    
}

int StructSymbol::getSize() const { return type_size; }
string StructSymbol::getName() const { return name; }
string StructSymbol::getScopeName() const { return scope_name; }

SymbolType StructSymbol::getSymbolType() const { return SymbolType::STRUCT; }
TypeKind StructSymbol::getTypeKind() const { return TypeKind::STRUCT; }

void StructSymbol::accept(ScopeVisitor *visitor) { visitor->visit(this); }

VarAllocator* StructSymbol::get_valloc() const { return &valloc; }
	
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
	string conversion_constructor_name = st -> getName();

	auto func_sym = st -> resolveMember(conversion_constructor_name);
	
	if ( func_sym == nullptr )
		return nullptr;

	auto conv_constr = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(func_sym) -> getType().type);

	auto info = conv_constr -> getTypeInfo();

	auto it = info.symbols.find({VariableType(this)});

	return it == std::end(info.symbols) ? nullptr : it -> second;
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

	auto conv_constr = getConversionConstructor(st);

	return conv_constr;
}
