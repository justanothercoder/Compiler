#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), call_info() { }

CallNode::~CallNode() { delete caller; }
    
Type* CallNode::getType() const { return call_info.callee->getTypeInfo().return_type; }
    
void CallNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    caller->check(template_sym, expr);

    auto caller_type = TypeHelper::removeReference(caller->getType());

    if ( caller_type->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	{
		if ( caller_type->getTypeKind() != TypeKind::STRUCT )
			throw SemanticError("caller is not a function.");

		call_info = CallHelper::callCheck("operator()", static_cast<StructSymbol*>(caller_type), params, template_sym, expr);
		GlobalHelper::setTypeHint(caller, call_info.callee);
	}
	else
	{
		auto ov_func = static_cast<OverloadedFunctionSymbol*>(caller_type);
		auto scope = ov_func->isMethod() ? static_cast<StructSymbol*>(TypeHelper::removeReference(ov_func->getBaseType())) : getScope();
		call_info = CallHelper::callCheck(ov_func->getName(), scope, params, template_sym, expr);
	    GlobalHelper::setTypeHint(caller, call_info.callee);
	}
}

void CallNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{    
    CodeGen::genCallCode(call_info, params, template_sym, expr,
			[&]()
			{
				caller->gen(template_sym, expr);

				if ( call_info.callee->getName() == "operator()" )
					CodeGen::emit("lea rax, [" + call_info.callee->getScopedTypedName() + "]");
			},
			[&]() { caller->gen(template_sym, expr); }
	);
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
