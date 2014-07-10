#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name
		                      , VariableType return_type
							  , FunctionTypeInfo function_type_info
							  , Scope *enclosing_scope
							  , FunctionTraits traits
							  , optional<CodeObject> code_obj) : code_obj(code_obj)
													           , return_type(return_type)
													           , function_type_info(function_type_info)
													           , name(name)
													           , enclosing_scope(enclosing_scope)
													           , traits(traits)
{
	scope_name = enclosing_scope->getScopeName() + "_" + (traits.is_operator ? GlobalHelper::getCodeOperatorName(name) : name);
}

string FunctionSymbol::getTypedName() const
{
	string res = (traits.is_operator ? GlobalHelper::getCodeOperatorName(name) : name);

	for ( auto type : function_type_info.params_types )
		res += "_" + type.getName();

	return res;
}

string FunctionSymbol::getScopedTypedName() const
{
	string res = scope_name;

	auto& pt = function_type_info.params_types;

	for ( auto type : pt )
		res += "_" + type.getName();

	return res;
}

Scope* FunctionSymbol::getEnclosingScope() const { return enclosing_scope; }
string FunctionSymbol::getScopeName() const { return scope_name; }

bool FunctionSymbol::isOperator() const { return traits.is_operator; }
bool FunctionSymbol::isMethod() const { return traits.is_method; }
bool FunctionSymbol::isConstructor() const { return traits.is_constructor; }

string FunctionSymbol::getName() const { return name; }
int FunctionSymbol::getSize() const { return GlobalConfig::int_size; }

FunctionTraits FunctionSymbol::getTraits() const { return traits; }

SymbolType FunctionSymbol::getSymbolType() const { return SymbolType::FUNCTION; }
TypeKind FunctionSymbol::getTypeKind() const { return TypeKind::FUNCTION; }

void FunctionSymbol::accept(ScopeVisitor *visitor) { visitor->visit(this); }
	
VarAllocator* FunctionSymbol::get_valloc() { return &valloc; }

TempAllocator& FunctionSymbol::getTempAlloc() { return temp_alloc; }
