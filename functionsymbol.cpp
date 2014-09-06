#include "functionsymbol.hpp"
#include "scopevisitor.hpp"
#include "functionsymboldefine.hpp"
#include "globalhelper.hpp"
#include "globalconfig.hpp"

FunctionSymbol::FunctionSymbol(std::string name
		                      , const Type *return_type
							  , FunctionTypeInfo function_type_info
							  , Scope *enclosing_scope
							  , FunctionTraits traits
							  , boost::optional<CodeObject> code_obj) : code_obj(code_obj)
													                  , return_type(return_type)
													                  , function_type_info(function_type_info)
															          , is_constexpr(false)
													                  , name(name)
													                  , enclosing_scope(enclosing_scope)
													                  , traits(traits)
{
	scope_name = enclosing_scope->getScopeName() + "_" + (traits.is_operator ? GlobalHelper::getCodeOperatorName(name) : name);

	GlobalHelper::has_definition[this] = false;
}

std::string FunctionSymbol::getTypedName() const
{
	auto res = (traits.is_operator ? GlobalHelper::getCodeOperatorName(name) : name);

	for ( auto type : function_type_info.params_types )
		res += "_" + type -> getName();

	return res;
}

std::string FunctionSymbol::getScopedTypedName() const
{
	auto res = scope_name;

	auto& pt = function_type_info.params_types;

	for ( auto type : pt )
		res += "_" + type -> getName();

	return res;
}

Scope* FunctionSymbol::getEnclosingScope() const 
{ 
	return enclosing_scope; 
}

std::string FunctionSymbol::getScopeName() const 
{ 
	return scope_name; 
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

void FunctionSymbol::accept(ScopeVisitor *visitor) 
{ 
	visitor -> visit(this); 
}
	
VarAllocator& FunctionSymbol::getVarAlloc() const 
{ 
	return var_alloc; 
}

TempAllocator& FunctionSymbol::getTempAlloc() const
{ 
	return temp_alloc; 
}

const TemplateInfo& FunctionSymbol::getTemplateInfo() const
{
	return template_info;
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
	
ScopeVisitor* FunctionSymbol::getScopeVisitor() 
{
	return new FunctionSymbolDefine(this);
}
