#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), resolved_function_symbol(nullptr) { }
    
Type* CallNode::getType() const { return resolved_function_symbol->getTypeInfo().getReturnType(); }
bool CallNode::isLeftValue() const { return false; }
    
void CallNode::check()
{
    caller->check();   

    Type *caller_type = caller->getType();
    if ( caller_type->isReference() )
	caller_type = static_cast<ReferenceType*>(caller_type)->getReferredType();
    
    if ( caller_type->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	throw SemanticError("caller is not a function.");

    for ( auto i : params )
	i->check();

    OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(caller_type);

    bool is_method = ov_func->isMethod();
    
    vector<Type*> params_types;  
    if ( is_method )
	params_types.push_back(ov_func->getBaseType());
    
    std::transform(std::begin(params), std::end(params), std::back_inserter(params_types), [](ExprNode *t) { return t->getType(); });
    
    resolved_function_symbol = FunctionHelper::getViableOverload(ov_func, params_types);

    if ( resolved_function_symbol == nullptr )
	throw SemanticError("No viable overload of '" + ov_func->getName() + "'.");	

    auto resolved_function_type_info = resolved_function_symbol->getTypeInfo();
    
    int is_meth = (is_method ? 1 : 0);
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
	if ( resolved_function_type_info.getParamType(i)->isReference() && !params[i - is_meth]->isLeftValue() )
	    throw SemanticError("parameter is not an lvalue.");
    }    
    
    GlobalHelper::setTypeHint(caller, resolved_function_symbol);
}

void CallNode::gen()
{
    caller->gen();    

    CodeGen::emit("push rsi");
    CodeGen::emit("mov rsi, rax");

    CodeGen::genCallCode(resolved_function_symbol, params);
    CodeGen::emit("pop rsi");
}

void CallNode::build_scope()
{
    caller->setScope(this->getScope());
    caller->build_scope();
    for ( auto i : params )
    {
	i->setScope(this->getScope());
	i->build_scope();
    }
}

void CallNode::template_check(TemplateStructSymbol *template_sym)
{
    caller->template_check(template_sym);

    for ( auto i : params )
	i->template_check(template_sym);

    Type *caller_type = caller->getType();

    if ( caller_type != nullptr )
    {    
	if ( caller_type->isReference() )
	    caller_type = static_cast<ReferenceType*>(caller_type)->getReferredType();
    
	if ( caller_type->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	    throw SemanticError("caller is not a function.");
    
	OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(caller_type);
	
	bool is_method = ov_func->isMethod();
	
	vector<Type*> params_types;  
	if ( is_method )
	    params_types.push_back(ov_func->getBaseType());
    
	std::transform(std::begin(params), std::end(params), std::back_inserter(params_types), [](ExprNode *t) { return t->getType(); });
    
	resolved_function_symbol = FunctionHelper::getViableOverload(ov_func, params_types);

	if ( resolved_function_symbol == nullptr )
	    throw SemanticError("No viable overload of '" + ov_func->getName() + "'.");	

	auto resolved_function_type_info = resolved_function_symbol->getTypeInfo();
    
	int is_meth = (is_method ? 1 : 0);
    
	for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
	{
	    if ( resolved_function_type_info.getParamType(i)->isReference() && !params[i - is_meth]->isLeftValue() )
		throw SemanticError("parameter is not an lvalue.");
	}    
    
	GlobalHelper::setTypeHint(caller, resolved_function_symbol);
    }
    else
    {
	
    }
}
