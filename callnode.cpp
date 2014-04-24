#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), call_info(), code_obj() { }

CallNode::~CallNode() { delete caller; }
    
void CallNode::check(const TemplateInfo& template_info)
{
    caller->check(template_info);

    auto caller_type = caller->getType().type;

    if ( caller_type->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	{
		if ( caller_type->getTypeKind() != TypeKind::STRUCT )
			throw SemanticError("caller is not a function.");

		call_info = CallHelper::callCheck("operator()", static_cast<StructSymbol*>(caller_type), params, template_info);
	}
	else
	{
		auto ov_func = static_cast<OverloadedFunctionSymbol*>(caller_type);
		auto scope = ov_func->isMethod() ? static_cast<StructSymbol*>(ov_func->getBaseType().type) : getScope();
		call_info = CallHelper::callCheck(ov_func->getName(), scope, params, template_info);
	}

	GlobalHelper::setTypeHint(caller, call_info.callee);
	
	getScope()->get_valloc()->addReturnValueSpace(getType().getSize());
	
	for ( auto param : params )
		getScope()->get_valloc()->addSpecialSpace(param);
}

CodeObject& CallNode::gen(const TemplateInfo& template_info)
{  
  	if ( call_info.callee->isMethod() )
		code_obj.genCallCode(call_info, params, template_info, caller->gen(template_info), caller->getType().is_ref);
	else
	{
		CodeObject empty;
		code_obj.genCallCode(call_info, params, template_info, empty, false);
	}

	return code_obj;
}

AST* CallNode::copyTree() const
{
    vector<ExprNode*> expr(params.size());
    std::transform(std::begin(params), std::end(params), std::begin(expr), [&] (ExprNode *ex) { return static_cast<ExprNode*>(ex->copyTree()); });
    return new CallNode(static_cast<ExprNode*>(caller->copyTree()), expr);
}

vector<AST*> CallNode::getChildren() const
{
	vector<AST*> vec { caller };
	vec.insert(std::begin(vec), std::begin(params), std::end(params));
	return vec;
}

VariableType CallNode::getType() const { return call_info.callee->function_type_info.return_type; }
bool CallNode::isLeftValue() const { return false; }
