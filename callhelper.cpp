#include "callhelper.hpp"

CallInfo CallHelper::callCheck(string name, Scope *sc, std::vector<ExprNode*> params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    for ( auto i : params )
		i->check(template_sym, expr);

	auto params_types = CallHelper::extractTypes(params);

	auto function_sym = CallHelper::resolveOverload(name, sc, params_types);
	
	if ( function_sym == nullptr )
		throw SemanticError("No viable overload of '" + name + "'.");  

    auto function_info = function_sym->getTypeInfo();
    
    int is_meth = (function_sym->isMethod() ? 1 : 0);
	
	for ( int i = function_info.params_types.size() - 1; i >= is_meth; --i )
    {
		if ( function_info.params_types.at(i)->isReference() && !params.at(i - is_meth)->isLeftValue() )
			throw SemanticError("parameter is not an lvalue.");
    }   

	return getCallInfo(function_sym, params);
}

CallInfo CallHelper::getCallInfo(FunctionSymbol *function_sym, std::vector<ExprNode*> params)
{
    auto function_info = function_sym->getTypeInfo();
	
	auto params_types = CallHelper::extractTypes(params);
   
	vector<ConversionInfo> conversions;
	vector<FunctionSymbol*> copy_constructors;

    int is_meth = (function_sym->isMethod() ? 1 : 0);
   
	for ( size_t i = is_meth; i < function_info.params_types.size(); ++i )
	{
		auto actual_type = params_types.at(i - is_meth);
		auto desired_type = function_info.params_types.at(i);

		bool is_left_value = params.at(i - is_meth)->isLeftValue();

		conversions.push_back(CallHelper::getConversionInfo(actual_type, desired_type, is_left_value));
		
		auto copy_constr = desired_type->isReference() ? nullptr : TypeHelper::getCopyConstructor(TypeHelper::removeReference(desired_type));
		copy_constructors.push_back(copy_constr);
	}
	
	return CallInfo(function_sym, params_types, conversions, copy_constructors); 
}

OverloadedFunctionSymbol* CallHelper::getOverloadedFunc(string name, Scope *scope)
{
	auto _ = scope->resolve(name);
	
	if ( _ == nullptr || _->getSymbolType() != SymbolType::VARIABLE )
		throw SemanticError("No such symbol " + name + ".");	

	return dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(_)->getType());
}

OverloadedFunctionSymbol* CallHelper::getOverloadedMethod(string name, StructSymbol *scope)
{
	auto _ = scope->resolveMember(name);
	
	if ( _ == nullptr || _->getSymbolType() != SymbolType::VARIABLE )
		throw SemanticError("No such symbol " + name + ".");	

	return dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(_)->getType());
}
	
FunctionSymbol* CallHelper::resolveOverload(string name, Scope *scope, std::vector<Type*> params_types)
{
	while ( scope != nullptr )
	{
		OverloadedFunctionSymbol *ov_func = nullptr;
	  
		try { ov_func = CallHelper::getOverloadedFunc(name, scope); }
		catch ( SemanticError& e ) { return nullptr; }

		auto pt = params_types;

		if ( ov_func->isMethod() )
			pt.insert(std::begin(pt), TypeHelper::addReference(ov_func->getBaseType()));

		auto func_sym = FunctionHelper::getViableOverload(ov_func, pt);

		if ( func_sym == nullptr )
		{
			try
			{
				while ( scope != nullptr && CallHelper::getOverloadedFunc(name, scope) == ov_func )
					scope = scope->getEnclosingScope();
				continue;
			}
			catch ( SemanticError& e ) { return nullptr; }
		}
		
		return func_sym; 
	}
	return nullptr;
}
	
std::vector<Type*> CallHelper::extractTypes(std::vector<ExprNode*> params)
{
	vector<Type*> params_types(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(params_types), [](ExprNode *t) { return t->getType(); });

	return params_types;
}

ConversionInfo CallHelper::getConversionInfo(Type *lhs, Type *rhs, bool is_lhs_left_value)
{
	auto _lhs = TypeHelper::removeReference(lhs);
	auto _rhs = TypeHelper::removeReference(rhs);

	if ( rhs->isReference() )
	{
		if ( _lhs != _rhs )
			throw SemanticError("Invalid initialization of '" + rhs->getName() + "' with type '" + lhs->getName() + "'");

		return ConversionInfo(nullptr, false, !lhs->isReference());
	}
	else
	{
		auto conv = (_lhs == _rhs) ? nullptr : TypeHelper::getConversion(_lhs, _rhs);
		return ConversionInfo(conv, lhs->isReference() && !is_lhs_left_value, false);
	}
}
