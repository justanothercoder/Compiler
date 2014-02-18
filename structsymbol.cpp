#include "structsymbol.hpp"

StructSymbol::StructSymbol(string name, Scope *enclosing_scope) : name(name), enclosing_scope(enclosing_scope)
{
	type_size = 0;
	scope_size = 0;

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
int StructSymbol::getScopeSize() const { return 0; }
string StructSymbol::getScopeName() const { return scope_name; }

SymbolType StructSymbol::getSymbolType() const { return SymbolType::STRUCT; }
TypeKind StructSymbol::getTypeKind() const { return TypeKind::STRUCT; }

void StructSymbol::accept(ScopeVisitor *visitor) { visitor->visit(this); }

int StructSymbol::getFreeAddress() const { return 0; }
