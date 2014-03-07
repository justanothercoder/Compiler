#include "callhelper.hpp"

FunctionSymbol* CallHelper::callCheck(string name, Scope *sc, const std::vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*>& expr)
{
    for ( auto i : params )
		i->check(template_sym, expr);
    
	auto resolved_function_symbol = CallHelper::resolveOverload(name, sc, params);

    if ( resolved_function_symbol == nullptr )
		throw SemanticError("No viable overload of '" + name + "'.");	

    auto resolved_function_type_info = resolved_function_symbol->getTypeInfo();
    
    int is_meth = (resolved_function_symbol->isMethod() ? 1 : 0);
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
		if ( resolved_function_type_info.getParamType(i)->isReference() && !params[i - is_meth]->isLeftValue() )
			throw SemanticError("parameter is not an lvalue.");
    }   
	
	return resolved_function_symbol; 
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
	
FunctionSymbol* CallHelper::resolveOverload(string name, Scope *sc, std::vector<Type*> params_types)
{
	auto scope = sc;

	while ( true )
	{
		OverloadedFunctionSymbol *ov_func = nullptr;
	  
		if ( scope == nullptr )
			return nullptr;

		try { ov_func = CallHelper::getOverloadedFunc(name, scope); }
		catch ( SemanticError& e ) { return nullptr; }

		auto pt = params_types;

		if ( ov_func->isMethod() )
			pt.insert(std::begin(pt), TypeHelper::getReferenceType(ov_func->getBaseType()));

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

FunctionSymbol* CallHelper::resolveOverload(string name, Scope *sc, std::vector<ExprNode*> params)
{
    vector<Type*> params_types;     
	std::transform(std::begin(params), std::end(params), std::back_inserter(params_types), [](ExprNode *t) { return t->getType(); });

	return resolveOverload(name, sc, params_types);
}
