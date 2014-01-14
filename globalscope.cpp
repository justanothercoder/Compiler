#include "globalscope.hpp"

GlobalScope::GlobalScope() 
{
    scope_size = 0;
    scope_address = 0;
}

void GlobalScope::define(Symbol *sym)
{
    string sym_name = sym->getName();
    
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	auto it = table.find(sym_name);
	
	if ( it == std::end(table) )
	    table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ })));

	Symbol *res_sym = table[sym_name];	

	if ( dynamic_cast<VariableSymbol*>(res_sym) == nullptr || dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(res_sym)->getType()) == nullptr )
	    throw SemanticError(sym_name + " is already defined.");

	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym_name + " is already defined as not function");
       	
	FunctionTypeInfo func_type_info = static_cast<FunctionSymbol*>(sym)->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(table[sym_name])->getType());
	ofs->addOverload(func_type_info, static_cast<FunctionSymbol*>(sym));
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	VariableSymbol *_sym = static_cast<VariableSymbol*>(sym);
	
	table[sym_name] = sym;
	addresses[_sym] = scope_size + sizeof(int*);
	scope_size += _sym->getType()->getSize();
    }
    else
	table[sym_name] = sym;
}


Scope* GlobalScope::getEnclosingScope() const { return nullptr; }
string GlobalScope::getScopeName() const { return ""; }
