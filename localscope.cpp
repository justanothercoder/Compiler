#include "localscope.hpp"

LocalScope::LocalScope(Scope *enclosing_scope) : enclosing_scope(enclosing_scope) { }

void LocalScope::define(Symbol *sym)
{
    string sym_name = sym->getName();
    
    if ( sym->getSymbolType() == SymbolType::FUNCTION )
    {
	FunctionSymbol *func = static_cast<FunctionSymbol*>(sym);
	
	auto it = table.find(sym_name);
	
	if ( it == std::end(table) )
	    table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), func->getTraits()));

	Symbol *res_sym = table[sym_name];	

	if ( res_sym->getSymbolType() != SymbolType::VARIABLE || static_cast<VariableSymbol*>(res_sym)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	    throw SemanticError(sym_name + " is already defined.");

//	OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());	
       	
	auto func_type_info = func->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());
	ofs->addOverload(func_type_info, func);
    }
    else if ( sym->getSymbolType() == SymbolType::VARIABLE )
    {
	table[sym_name] = sym;
	addresses[static_cast<VariableSymbol*>(sym)] = scope_size + GlobalConfig::int_size;
	scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize();
    }
    else
	table[sym_name] = sym;
}

Scope* LocalScope::getEnclosingScope() const { return enclosing_scope; }
string LocalScope::getScopeName() const { return ""; }
void LocalScope::recalc_scope_address() { scope_address = enclosing_scope->getScopeAddress(); }
