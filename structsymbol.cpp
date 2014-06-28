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
	string conversion_constructor_name = st -> getName();

	auto conv_constr = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(st -> resolveMember(conversion_constructor_name)) -> getType().type);

	auto info = conv_constr -> getTypeInfo();

	auto it = info.symbols.find({VariableType(this)});

	return it != std::end(info.symbols);
}

bool StructSymbol::hasConversionOperator(StructSymbol *st)
{
	string cast_operator_name = "operator " + st -> getName();
	FunctionSymbol *cast_operator = CallHelper::resolveOverload(cast_operator_name, this, { });

	return cast_operator != nullptr;
}
	
