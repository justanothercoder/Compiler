#include "functionsymbol.hpp"
#include "scopevisitor.hpp"
#include "functionsymboldefine.hpp"
#include "globalconfig.hpp"

FunctionSymbol::FunctionSymbol(std::string name
		                      , const Type *return_type
							  , FunctionTypeInfo function_type_info
							  , Scope *enclosing_scope
							  , FunctionTraits traits
							  , boost::optional<CodeObject> code_obj) : FunctionScope(enclosing_scope -> getScopeName() + "_" + (traits.is_operator ? GlobalConfig::getCodeOperatorName(name) : name), enclosing_scope),
								  code_obj(code_obj)
													                  , return_type(return_type)
													                  , function_type_info(function_type_info)
															          , is_constexpr(false)
													                  , name(name)
													                  , traits(traits)
{

}

std::string FunctionSymbol::getTypedName() const
{
	auto res = (traits.is_operator ? GlobalConfig::getCodeOperatorName(name) : name);

	for ( auto type : function_type_info.params_types )
		res += "_" + type -> getName();

	return res;
}

std::string FunctionSymbol::getScopedTypedName() const
{
	auto res = getScopeName(); //scope_name;

	auto& pt = function_type_info.params_types;

	for ( auto type : pt )
		res += "_" + type -> getName();

	return res;
}

bool FunctionSymbol::isOperator() const 
{ 
	return traits.is_operator; 
}

bool FunctionSymbol::isMethod() const 
{ 
	return traits.is_method; 
}

bool FunctionSymbol::isConstructor() const 
{ 
	return traits.is_constructor; 
}

std::string FunctionSymbol::getName() const 
{ 
	return name; 
}

size_t FunctionSymbol::getSize() const 
{ 
	return GlobalConfig::int_size; 
}

FunctionTraits FunctionSymbol::getTraits() const 
{ 
	return traits; 
}

SymbolType FunctionSymbol::getSymbolType() const 
{ 
	return SymbolType::FUNCTION; 
}

TypeKind FunctionSymbol::getTypeKind() const 
{ 
	return TypeKind::FUNCTION; 
}

bool FunctionSymbol::isConvertableTo(const Type *) const 
{
	return false;
}
	
boost::optional<int> FunctionSymbol::rankOfConversion(const Type *) const 
{
	return boost::none;
}
	
FunctionSymbol* FunctionSymbol::getConversionTo(const Type *) const 
{
	return nullptr;
}

const Symbol* FunctionSymbol::getSymbol() const
{
	return this;
}
	
ScopeVisitor& FunctionSymbol::getScopeVisitor() 
{
	return *(new FunctionSymbolDefine(this));
}
	
bool FunctionSymbol::isUnsafeBlock() const
{
	return is_unsafe;
}
