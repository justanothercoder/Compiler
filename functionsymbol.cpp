#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, bool is_operator, bool is_method) : name(name), function_type_info(function_type_info), enclosing_scope(enclosing_scope), is_operator(is_operator), is_method(is_method)
{
    scope_size = 0;
    params_size = GlobalConfig::int_size;
    
    scope_name = enclosing_scope->getScopeName() + "_" + name;
}

void FunctionSymbol::define(Symbol *sym)
{
    string sym_name = sym->getName();
	
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	if ( table[sym_name] == nullptr )
	    table[sym_name] = new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }));

	OverloadedFunctionSymbol* ov_func = dynamic_cast<OverloadedFunctionSymbol*>(table[sym_name]);
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym_name + " is not a function");
	
	OverloadedFunctionTypeInfo type_info = ov_func->getTypeInfo();
	
	FunctionTypeInfo func_type_info = static_cast<FunctionSymbol*>(sym)->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(table[sym_name])->getType());
	ofs->addOverload(func_type_info, static_cast<FunctionSymbol*>(sym));
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	table[sym_name] = sym;
	if ( static_cast<VariableSymbol*>(sym)->isParam() )
	    addresses[static_cast<VariableSymbol*>(sym)] = -(params_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
	else
	{
	    addresses[static_cast<VariableSymbol*>(sym)] = scope_size + GlobalConfig::int_size;
	    scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize();
	}
    }
    else
	table[sym_name] = sym;
}

string FunctionSymbol::getTypedName() const
{
    string res = name;

    for ( int i = 0; i < function_type_info.getNumberOfParams(); ++i )
	res += "_" + function_type_info.getParamType(i)->getName();

    return res;
}

string FunctionSymbol::getScopedTypedName() const
{
    string res = scope_name;

    for ( int i = 0; i < function_type_info.getNumberOfParams(); ++i )
	res += "_" + function_type_info.getParamType(i)->getName();

    return res;
}

Scope* FunctionSymbol::getEnclosingScope() const { return enclosing_scope; }
int FunctionSymbol::getScopeSize() const { return scope_size + params_size; }
string FunctionSymbol::getScopeName() const { return scope_name; }

void FunctionSymbol::recalc_scope_address() { scope_address = enclosing_scope->getScopeAddress() + enclosing_scope->getScopeSize() + GlobalConfig::int_size; }

bool FunctionSymbol::isOperator() const { return is_operator; }
bool FunctionSymbol::isMethod() const { return is_method; }

string FunctionSymbol::getName() const { return name; }
int FunctionSymbol::getSize() const { return GlobalConfig::int_size; }

FunctionTypeInfo FunctionSymbol::getTypeInfo() const { return function_type_info; }
void FunctionSymbol::setTypeInfo(FunctionTypeInfo function_type_info) { this->function_type_info = function_type_info; }

