#include "globalscope.hpp"

GlobalScope::GlobalScope() 
{
    scope_size = 0;
    scope_address = 0;
}

void GlobalScope::define(Symbol *sym)
{
    string sym_name = sym->getName();

    if ( sym->getSymbolType() == SymbolType::FUNCTION )    
    {
	FunctionSymbol *func = static_cast<FunctionSymbol*>(sym);
	
	auto it = table.find(sym_name);
	
	if ( it == std::end(table) )
	    table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), func->getTraits()));

	Symbol *res_sym = table[sym_name];	
	if ( res_sym->getSymbolType() != SymbolType::VARIABLE )
	    throw SemanticError(sym_name + " is already defined.");

	if ( static_cast<VariableSymbol*>(res_sym)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	    throw SemanticError(sym_name + " is already defined as not function");

	auto func_type_info = func->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());
	ofs->addOverload(func_type_info, func);
    }
    else if ( sym->getSymbolType() == SymbolType::VARIABLE )
    {
	VariableSymbol *_sym = static_cast<VariableSymbol*>(sym);
	
	table[sym_name] = sym;
	addresses[_sym] = scope_size + GlobalConfig::int_size;
	scope_size += _sym->getType()->getSize();
    }
    else
	table[sym_name] = sym;
}


Scope* GlobalScope::getEnclosingScope() const { return nullptr; }
string GlobalScope::getScopeName() const { return ""; }
