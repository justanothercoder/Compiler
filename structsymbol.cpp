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

void StructSymbol::define(Symbol *sym)
{
    string sym_name = sym->getName();
    
    if ( sym->getSymbolType() == SymbolType::FUNCTION )
    {
	FunctionSymbol *func_sym = static_cast<FunctionSymbol*>(sym);
	
	auto it = table.find(sym_name);
	
	if ( it == std::end(table) )
	    table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), func_sym->getTraits()));

	Symbol *res_sym = table[sym_name];	

	if ( res_sym->getSymbolType() != SymbolType::VARIABLE || static_cast<VariableSymbol*>(res_sym)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	    throw SemanticError(sym_name + " is already defined.");

	auto func_type_info = func_sym->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());
	ofs->addOverload(func_type_info, func_sym);
    }
    else if ( sym->getSymbolType() == SymbolType::VARIABLE )
    {
	table[sym_name] = sym;

	VariableSymbol *_sym = static_cast<VariableSymbol*>(sym);
	
	addresses[_sym] = scope_size;
	scope_size += _sym->getType()->getSize();

	type_size += _sym->getType()->getSize();
    }
    else
	table[sym_name] = sym;
}

int StructSymbol::getSize() const { return type_size; }
string StructSymbol::getName() const { return name; }
int StructSymbol::getScopeSize() const { return 0; }
string StructSymbol::getScopeName() const { return scope_name; }

SymbolType StructSymbol::getSymbolType() const { return SymbolType::STRUCT; }
TypeKind StructSymbol::getTypeKind() const { return TypeKind::STRUCT; }
