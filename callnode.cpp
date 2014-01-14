#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), resolved_function_type_info(nullptr, { })
{
    
}
    
Type* CallNode::getType() const
{
    return resolved_function_type_info.getReturnType();
}

bool CallNode::isLeftValue() const
{
    return false;
}
    
void CallNode::check()
{
    caller->check();   

    Type *caller_type = caller->getType();
    if ( dynamic_cast<ReferenceType*>(caller_type) )
	caller_type = static_cast<ReferenceType*>(caller_type)->getReferredType();
    
    OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(caller_type);
    
    if ( ov_func == nullptr )
	throw SemanticError("caller is not a function.");

    vector<Type*> params_types;  

    bool is_method = ov_func->isMethod();

    if ( is_method )
	params_types.push_back(ov_func->getBaseType());

    for ( auto it : params )
    {
	it->check();
	params_types.push_back(it->getType());
    }

    auto overloads = FunctionHelper::getBestOverload(ov_func->getTypeInfo().overloads, params_types);

    if ( overloads.empty() )
	throw SemanticError("No viable overload of '" + ov_func->getName() + "'.");

    resolved_function_type_info = *std::begin(overloads);

    int is_meth = (is_method ? 1 : 0);
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
	if ( dynamic_cast<ReferenceType*>(resolved_function_type_info.getParamType(i)) && !params[i - is_meth]->isLeftValue() )
	    throw SemanticError("parameter is not an lvalue.");
    }    
    
    GlobalHelper::setTypeHint(caller, ov_func);
}

void CallNode::gen()
{
    int paramsSize = 0;

    bool is_method = resolved_function_type_info.isMethod();

    int is_meth = (is_method ? 1 : 0);

    caller->gen();    
    CodeGen::emit("mov rsi, rax");
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
	params[i - is_meth]->gen();
	if ( dynamic_cast<ReferenceType*>(resolved_function_type_info.getParamType(i)) )
	{	    
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rax");
	    paramsSize += sizeof(int*);
	}
	else
	{
	    for ( int j = 0; j < resolved_function_type_info.getParamType(i)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	    {	    
		CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
		CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rbx");
	    }
	}
    }

    if ( is_method )
    {
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rdi");
	paramsSize += sizeof(int*);
    }

    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));
    
    CodeGen::emit("call rsi");
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}

void CallNode::build_scope()
{
    caller->scope = scope;
    caller->build_scope();
    for ( auto i : params )
    {
	i->scope = scope;
	i->build_scope();
    }
}
