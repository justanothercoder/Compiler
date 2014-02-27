#include "callhelper.hpp"

FunctionSymbol* CallHelper::callCheck(OverloadedFunctionSymbol *ov_func, const std::vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*>& expr)
{
    for ( auto i : params )
		i->check(template_sym, expr);
    
	bool is_method = ov_func->isMethod();
	
    vector<Type*> params_types;  
    if ( is_method )
		params_types.push_back(ov_func->getBaseType());
    
    std::transform(std::begin(params), std::end(params), std::back_inserter(params_types), [](ExprNode *t) { return t->getType(); });
    
	auto resolved_function_symbol = FunctionHelper::getViableOverload(ov_func, params_types);

    if ( resolved_function_symbol == nullptr )
		throw SemanticError("No viable overload of '" + ov_func->getName() + "'.");	

    auto resolved_function_type_info = resolved_function_symbol->getTypeInfo();
    
    int is_meth = (is_method ? 1 : 0);
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
		if ( resolved_function_type_info.getParamType(i)->isReference() && !params[i - is_meth]->isLeftValue() )
			throw SemanticError("parameter is not an lvalue.");
    }   
	
	return resolved_function_symbol; 
}

OverloadedFunctionSymbol* CallHelper::getOverloadedFunc(string name, Scope *scope)
{
	Symbol *_sym = scope->resolve(name);
	
	if ( dynamic_cast<VariableSymbol*>(_sym) == nullptr )
		throw SemanticError("No such symbol " + name);	

	return dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(_sym)->getType());
}

OverloadedFunctionSymbol* CallHelper::getOverloadedMethod(string name, StructSymbol *scope)
{
	Symbol *_sym = scope->resolveMember(name);
	
	if ( dynamic_cast<VariableSymbol*>(_sym) == nullptr )
		throw SemanticError("No such symbol " + name);	

	return dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(_sym)->getType());
}
