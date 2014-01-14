#include "globalscope.hpp"

GlobalScope::GlobalScope()
{
    scope_size = 0;
    scope_address = 0;
}

void GlobalScope::define(Symbol *sym)
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
	addresses[static_cast<VariableSymbol*>(sym)] = scope_size + sizeof(int*);
	scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize();
    }
    else
	table[sym->getName()] = sym;
}


Scope* GlobalScope::getEnclosingScope() const
{
    return nullptr;
}

string GlobalScope::getScopeName() const
{
    return "";
}
