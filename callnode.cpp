#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), resolved_function_type_info(nullptr, { })
{
    
}
    
Type* CallNode::getType()
{
    return resolved_function_type_info.getReturnType();
}

bool CallNode::isLeftValue()
{
    return false;
}
    
void CallNode::check()
{
    caller->check();   

    Type *caller_type = caller->getType();
    while ( dynamic_cast<ReferenceType*>(caller_type) )
	caller_type = static_cast<ReferenceType*>(caller_type)->getReferredType();
    
    OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(caller_type);
    
    if ( ov_func == nullptr )
	throw SemanticError("caller is not a function");

    vector<Type*> params_types;

    for ( auto i : params )
    {
	i->check();
	params_types.push_back(i->getType());
    }

    auto overloads = FunctionHelper::getBestOverload(ov_func->getTypeInfo().overloads, params_types);

    if ( overloads.empty() )
	throw SemanticError("No viable overload");

    resolved_function_type_info = *std::begin(overloads);

    scope->setTypeHint(caller, ov_func);
}

void CallNode::gen()
{
    int paramsSize = 0;
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= 0; --i )
    {
	params[i]->gen();
	for ( int j = 0; j < resolved_function_type_info.getParamType(i)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	{
	    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize) + "], rbx");
	}
    }

    CodeGen::emit("sub rsp, " + std::to_string(paramsSize - sizeof(int*)));
    
    caller->gen();
    CodeGen::emit("call rax");
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}

void CallNode::build_scope()
{
    caller->scope = scope;
    for ( auto i : params )
	i->scope = scope;
}
