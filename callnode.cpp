#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), call_info(), code_obj() { }

CallNode::~CallNode() { delete caller; }
    
void CallNode::check()
{
    caller -> check();

    auto caller_type = caller -> getType().type;

    if ( caller_type -> getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	{
		if ( caller_type -> getTypeKind() != TypeKind::STRUCT )
			throw SemanticError("caller is not a function.");

		call_info = CallHelper::callCheck("operator()", static_cast<StructSymbol*>(caller_type), params);
	}
	else
	{
		auto ov_func = static_cast<OverloadedFunctionSymbol*>(caller_type);
		auto _scope = ov_func -> isMethod() ? static_cast<StructSymbol*>(ov_func -> getBaseType().type) : scope;
		call_info = CallHelper::callCheck(ov_func -> getName(), _scope, params);
	}

	caller -> type_hint = call_info.callee;
	
	scope -> get_valloc() -> addReturnValueSpace(getType().getSize());
	
	for ( auto param : params )
		scope -> get_valloc() -> addSpecialSpace(param);
}

CodeObject& CallNode::gen()
{  
  	if ( call_info.callee -> isMethod() )
		code_obj.genCallCode(call_info, params, caller -> gen(), caller -> getType().is_ref);
	else
	{
		CodeObject empty;
		code_obj.genCallCode(call_info, params, empty, false);
	}

	return code_obj;
}

AST* CallNode::copyTree() const
{
    vector<ExprNode*> expr(params.size());
    std::transform(std::begin(params), std::end(params), std::begin(expr), [&] (ExprNode *ex) { return static_cast<ExprNode*>(ex -> copyTree()); });
    return new CallNode(static_cast<ExprNode*>(caller -> copyTree()), expr);
}

vector<AST*> CallNode::getChildren() const
{
	vector<AST*> vec { caller };
	vec.insert(std::begin(vec), std::begin(params), std::end(params));
	return vec;
}

VariableType CallNode::getType() const { return call_info.callee -> function_type_info.return_type; }
bool CallNode::isLeftValue() const { return false; }
