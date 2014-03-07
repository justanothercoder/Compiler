#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), resolved_function_symbol(nullptr) { }

CallNode::~CallNode() { delete caller; }
    
Type* CallNode::getType() const { return resolved_function_symbol->getTypeInfo().getReturnType(); }
    
void CallNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    caller->check(template_sym, expr);

    auto caller_type = TypeHelper::removeReference(caller->getType());

    if ( caller_type->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	{
		if ( caller_type->getTypeKind() != TypeKind::STRUCT )
			throw SemanticError("caller is not a function.");

		resolved_function_symbol = CallHelper::callCheck("operator()", static_cast<StructSymbol*>(caller_type), params, template_sym, expr);
		GlobalHelper::setTypeHint(caller, resolved_function_symbol);
	}
	else
	{
		resolved_function_symbol = CallHelper::callCheck(static_cast<OverloadedFunctionSymbol*>(caller_type)->getName(), getScope(), params, template_sym, expr);
	    GlobalHelper::setTypeHint(caller, resolved_function_symbol);
	}
}

void CallNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{    
    caller->gen(template_sym, expr);

	if ( resolved_function_symbol->getName() == "operator()" )
		CodeGen::emit("lea rax, [" + resolved_function_symbol->getScopedTypedName() + "]");
    CodeGen::emit("push rsi");
    CodeGen::emit("mov rsi, rax");

    CodeGen::genCallCode(resolved_function_symbol, params, template_sym, expr);
    CodeGen::emit("pop rsi");
	if ( resolved_function_symbol->isMethod() )
		CodeGen::emit("pop rdi");
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
