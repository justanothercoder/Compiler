#include "callhelper.hpp"

CallInfo CallHelper::callCheck(string name, Scope *sc, const std::vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*>& expr)
{
    for ( auto i : params )
		i->check(template_sym, expr);

	vector<Type*> params_types(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(params_types), [](ExprNode *t) { return t->getType(); });

	auto resolved_function_symbol = CallHelper::resolveOverload(name, sc, params_types);

    if ( resolved_function_symbol == nullptr )
		throw SemanticError("No viable overload of '" + name + "'.");	

    auto function_info = resolved_function_symbol->getTypeInfo();
   
	vector<ConversionInfo> conversions;
	vector<FunctionSymbol*> copy_constructors;

    int is_meth = (resolved_function_symbol->isMethod() ? 1 : 0);
   
	for ( size_t i = is_meth; i < function_info.params_types.size(); ++i )
	{
		auto actual_type = params_types.at(i - is_meth);
		auto desired_type = function_info.params_types.at(i);
		
		auto _actual = TypeHelper::removeReference(actual_type);
		auto _desired = TypeHelper::removeReference(desired_type);

		if ( desired_type->isReference() )
		{
			if ( _actual != _desired )
				throw SemanticError("Invalid initialization of '" + desired_type->getName() + "' with type '" + actual_type->getName() + "'");

			conversions.push_back(ConversionInfo(nullptr, false, !actual_type->isReference()));

			auto copy_constr = desired_type->isReference() ? nullptr : TypeHelper::getCopyConstructor(_desired);
			copy_constructors.push_back(copy_constr);
		}
		else
		{
			auto conv = (_actual == _desired) ? nullptr : TypeHelper::getConversion(_actual, _desired);
			conversions.push_back(ConversionInfo(conv, actual_type->isReference(), false));

			auto copy_constr = desired_type->isReference() ? nullptr : TypeHelper::getCopyConstructor(_desired);
			copy_constructors.push_back(copy_constr);
		}
	}
	
    for ( int i = function_info.params_types.size() - 1; i >= is_meth; --i )
    {
		if ( function_info.params_types.at(i)->isReference() && !params.at(i - is_meth)->isLeftValue() )
			throw SemanticError("parameter is not an lvalue.");
    }   
	
	return CallInfo(resolved_function_symbol, params_types, conversions, copy_constructors); 
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
}
