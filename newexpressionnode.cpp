#include "newexpressionnode.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params) : type_info(type_info), params(params), call_info() { }

NewExpressionNode::~NewExpressionNode()
{
	for ( auto i : params )
		delete i;
}

void NewExpressionNode::check(const TemplateInfo& template_info)
{
	string name = type_info.type_name;

	auto type = static_cast<StructSymbol*>(TypeHelper::fromTypeInfo(type_info, getScope(), template_info).type);

	getScope()->get_valloc()->addLocal(type->getSize());

	call_info = CallHelper::callCheck(name, type, params, template_info); 
}

void NewExpressionNode::gen(const TemplateInfo& template_info)
{
	CodeGen::genCallCode(call_info, params, template_info,
			[&]() {  CodeGen::emit("lea rax, [" + call_info.callee->getScopedTypedName() + "]"); },
			[&]() { CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "]"); }
	);
}

VariableType NewExpressionNode::getType() const { return call_info.callee->getTypeInfo().return_type; }

AST* NewExpressionNode::copyTree() const 
{
	vector<ExprNode*> vec(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(vec), [&](ExprNode *e) { return static_cast<ExprNode*>(e->copyTree()); });

	return new NewExpressionNode(type_info, vec);
}

vector<AST*> NewExpressionNode::getChildren() const { return vector<AST*>(std::begin(params), std::end(params)); }
