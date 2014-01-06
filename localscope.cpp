#include "localscope.hpp"

LocalScope::LocalScope(Scope *enclosing_scope) : enclosing_scope(enclosing_scope)
{
    
}

Scope* LocalScope::getEnclosingScope()
{
    return enclosing_scope;
}

void LocalScope::define(Symbol *sym)
{
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	auto it = table.find(sym->getName());
	
	if ( it == std::end(table) )
	    table[sym->getName()] = new VariableSymbol(sym->getName(), new OverloadedFunctionSymbol(sym->getName(), OverloadedFunctionTypeInfo({ })));

	Symbol *res_sym = table[sym->getName()];	

	if ( dynamic_cast<VariableSymbol*>(res_sym) == nullptr || dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(res_sym)->getType()) == nullptr )
	    throw SemanticError(sym->getName() + " is already defined.");

	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym->getName() + " is already defined as not function");
       	
	FunctionTypeInfo func_type_info = static_cast<FunctionSymbol*>(sym)->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(table[sym->getName()])->getType());
	ofs->addOverload(func_type_info, static_cast<FunctionSymbol*>(sym));
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	table[sym->getName()] = sym;
	addresses[static_cast<VariableSymbol*>(sym)] = (scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
    }
    else
	table[sym->getName()] = sym;
}

string LocalScope::getScopeName()
{
    return "";
}

void LocalScope::recalc_scope_address()
{
//    scope_address = enclosing_scope->getScopeAddress() + enclosing_scope->getScopeSize() + sizeof(int*);
    scope_address = enclosing_scope->getScopeAddress();
}
