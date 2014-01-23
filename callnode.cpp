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

    OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(caller_type);
    
    vector<Type*> params_types;  

    bool is_method = ov_func->isMethod();

    if ( is_method )
	params_types.push_back(ov_func->getBaseType());

    for ( auto it : params )
    {
	it->check();
	params_types.push_back(it->getType());
    }

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

    FunctionHelper::genCallCode(resolved_function_symbol, params);
    CodeGen::emit("pop rsi");
}

void CallNode::build_scope()
{
    caller->setScope(getScope());
    caller->build_scope();
    for ( auto i : params )
    {
	i->setScope(getScope());
	i->build_scope();
    }
}
