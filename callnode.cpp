#include "callnode.hpp"

CallNode::CallNode(ExprNode *caller, const vector<ExprNode*>& params) : caller(caller), params(params), resolved_function_symbol(nullptr) { }

CallNode::~CallNode() { delete caller; }
    
Type* CallNode::getType() const { return resolved_function_symbol->getTypeInfo().getReturnType(); }
    
void CallNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    caller->check(template_sym, expr);

    Type *caller_type = caller->getType();

    if ( caller_type->isReference() )
		caller_type = static_cast<ReferenceType*>(caller_type)->getReferredType();
    
    if ( caller_type->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
		throw SemanticError("caller is not a function.");

	resolved_function_symbol = CallHelper::callCheck(static_cast<OverloadedFunctionSymbol*>(caller_type), params, template_sym, expr);
    GlobalHelper::setTypeHint(caller, resolved_function_symbol);
}

void CallNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{    
    caller->gen(template_sym, expr);

    CodeGen::emit("push rsi");
    CodeGen::emit("mov rsi, rax");

    CodeGen::genCallCode(resolved_function_symbol, params, template_sym, expr);
    CodeGen::emit("pop rsi");
	if ( resolved_function_symbol->isMethod() )
		CodeGen::emit("pop rdi");
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

AST* CallNode::copyTree() const
{
    vector<ExprNode*> expr(params.size());

    std::transform(std::begin(params), std::end(params), std::begin(expr), [&] (ExprNode *ex) { return static_cast<ExprNode*>(ex->copyTree()); });
    
    return new CallNode(static_cast<ExprNode*>(caller->copyTree()), expr);
}

vector<AST*> CallNode::getChildren() const
{
	vector<AST*> vec;

	vec.push_back(caller);
	vec.insert(std::begin(vec), std::begin(params), std::end(params));

	return vec;
}
