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
    
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	FunctionSymbol *func_sym = static_cast<FunctionSymbol*>(sym);
	
	auto it = table.find(sym_name);
	
	if ( it == std::end(table) )
	    table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), func_sym->isMethod()));

	Symbol *res_sym = table[sym_name];	

	if ( dynamic_cast<VariableSymbol*>(res_sym) == nullptr || dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(res_sym)->getType()) == nullptr )
	    throw SemanticError(sym_name + " is already defined.");

	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(res_sym)->getType());
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym_name + " is already defined as not function");
       	
	FunctionTypeInfo func_type_info = func_sym->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(table[sym_name])->getType());
	ofs->addOverload(func_type_info, func_sym);
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	table[sym_name] = sym;

	if ( !static_cast<VariableSymbol*>(sym)->isField() )
	    throw SemanticError("internal error.");

	addresses[static_cast<VariableSymbol*>(sym)] = scope_size;
	scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize();

	type_size += static_cast<VariableSymbol*>(sym)->getType()->getSize();
    }
    else
	table[sym_name] = sym;
}

int StructSymbol::getSize() const { return type_size; }
string StructSymbol::getName() const { return name; }
int StructSymbol::getScopeSize() const { return 0; }
string StructSymbol::getScopeName() const { return scope_name; }
