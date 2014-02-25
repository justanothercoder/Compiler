#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits) : name(name), function_type_info(function_type_info), enclosing_scope(enclosing_scope), traits(traits)
{
	scope_size = 0;
	params_size = GlobalConfig::int_size;

	scope_name = enclosing_scope->getScopeName() + "_" + ((traits.is_operator && name == "operator[]") ? "operatorelem" : name);
}

string FunctionSymbol::getTypedName() const
{
	string res = name;

	if ( traits.is_operator )
	{
		if ( name == "operator[]" )
			res = "operatorelem";
	}

	auto& pt = function_type_info.getParamsTypes();

	for ( auto type : pt )
		res += "_" + type->getName();

	return res;
}

string FunctionSymbol::getScopedTypedName() const
{
	string res = scope_name;

	auto& pt = function_type_info.getParamsTypes();

	for ( auto type : pt )
		res += "_" + type->getName();

	return res;
}

Scope* FunctionSymbol::getEnclosingScope() const { return enclosing_scope; }
int FunctionSymbol::getScopeSize() const { return scope_size + params_size; }
string FunctionSymbol::getScopeName() const { return scope_name; }

void FunctionSymbol::recalc_scope_address() { scope_address = enclosing_scope->getScopeAddress() + enclosing_scope->getScopeSize() + GlobalConfig::int_size; }

bool FunctionSymbol::isOperator() const { return traits.is_operator; }
bool FunctionSymbol::isMethod() const { return traits.is_method; }
bool FunctionSymbol::isConstructor() const { return traits.is_constructor; }

string FunctionSymbol::getName() const { return name; }
int FunctionSymbol::getSize() const { return GlobalConfig::int_size; }

FunctionTypeInfo FunctionSymbol::getTypeInfo() const { return function_type_info; }
void FunctionSymbol::setTypeInfo(FunctionTypeInfo function_type_info) { this->function_type_info = function_type_info; }

FunctionTraits FunctionSymbol::getTraits() const { return traits; }

SymbolType FunctionSymbol::getSymbolType() const { return SymbolType::FUNCTION; }
TypeKind FunctionSymbol::getTypeKind() const { return TypeKind::FUNCTION; }

void FunctionSymbol::accept(ScopeVisitor *visitor) { visitor->visit(this); }
	
int FunctionSymbol::getFreeAddress() const { return getScopeSize() + GlobalConfig::int_size; }
	
void FunctionSymbol::increaseScopeTreeSize(int size) { scope_tree_size += size; }
