#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params)
{
    
}
    
Type* CallNode::getType()
{
    return resolved_function_type->getReturnType();
}

bool CallNode::isLeftValue()
{
    return false;
}
    
void CallNode::check()
{
    caller->check();   

    if ( dynamic_cast<FunctionType*>(caller->getType()) == nullptr && dynamic_cast<OverloadedFunctionType*>(caller->getType()) == nullptr )
	throw;

    vector<Type*> params_types;

    for ( auto i : params )
	i->check();

    std::transform(std::begin(params), std::end(params), std::back_inserter(params_types), [](ExprNode* expr) { return expr->getType(); });

    auto overloads = FunctionHelper::getBestOverload(dynamic_cast<OverloadedFunctionType*>(caller->getType())->overloads, params_types);
//    auto overloads = FunctionHelper::getBestOverload({dynamic_cast<FunctionType*>(caller->getType())}, params_types);

    if ( overloads.empty() )
	throw;

    resolved_function_type = *std::begin(overloads);

    scope->setTypeHint(caller, resolved_function_type);
    
//    resolved_function_type = dynamic_cast<FunctionType*>(caller->getType());
    
}

void CallNode::gen()
{
    int paramsSize = 0;
    
    for ( int i = resolved_function_type->getNumberOfParams() - 1; i >= 0; --i )
    {
	params[i]->gen();
	for ( int j = 0; j < resolved_function_type->getParamType(i)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	{
	    CodeGen::emit("mov rbx, [rax - " + std::to_string(paramsSize) + "]");
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize) + "], rbx");
	}
    }

    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));
    
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
