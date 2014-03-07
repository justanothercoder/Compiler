#include "newexpressionnode.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params) : type_info(type_info), params(params), resolved_constructor(nullptr) { }

NewExpressionNode::~NewExpressionNode()
{
	for ( auto i : params )
		delete i;
}

void NewExpressionNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	string name = type_info.getTypeName();

	auto type = static_cast<StructSymbol*>(TypeHelper::fromTypeInfo(type_info, getScope()));

	resolved_constructor = CallHelper::callCheck(name, type, params, template_sym, expr); 
}

void NewExpressionNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	CodeGen::emit("push rdi");
	CodeGen::emit("lea rdi, [rsp - " + std::to_string(GlobalConfig::int_size + resolved_constructor->getParamsSize()) + "]");
	CodeGen::construct_object(resolved_constructor, params, template_sym, expr);
	CodeGen::emit("pop rdi");
}

Type* NewExpressionNode::getType() const { return resolved_constructor->getTypeInfo().return_type; }

AST* NewExpressionNode::copyTree() const 
{
	vector<ExprNode*> vec(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(vec), [&](ExprNode *e) { return static_cast<ExprNode*>(e->copyTree()); });

	return new NewExpressionNode(type_info, vec);
}

vector<AST*> NewExpressionNode::getChildren() const { return vector<AST*>(std::begin(params), std::end(params)); }
