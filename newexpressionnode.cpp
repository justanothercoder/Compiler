#include "newexpressionnode.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params) : type_info(type_info), params(params), call_info(), code_obj() { }

NewExpressionNode::~NewExpressionNode()
{
	for ( auto i : params )
		delete i;
}

void NewExpressionNode::check()
{
	string name = type_info.type_name;

	auto type = static_cast<StructSymbol*>(TypeHelper::fromTypeInfo(type_info, scope, template_info).type);

	call_info = CallHelper::callCheck(name, type, params); 

	scope -> get_valloc() -> addLocal(this, type -> getSize());
	
	scope -> get_valloc() -> addReturnValueSpace(getType().getSize());
	for ( auto param : params )
		scope -> get_valloc() -> addSpecialSpace(param);
}

CodeObject& NewExpressionNode::gen()
{
	CodeObject new_place;
	new_place.emit("lea rax, [rbp - " + std::to_string(scope -> get_valloc() -> getAddress(this)) + "]");

	code_obj.genCallCode(call_info, params, new_place, false);

	return code_obj;
}

AST* NewExpressionNode::copyTree() const 
{
	vector<ExprNode*> vec(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(vec), [&](ExprNode *e) { return static_cast<ExprNode*>(e -> copyTree()); });

	return new NewExpressionNode(type_info, vec);
}

vector<AST*> NewExpressionNode::getChildren() const { return vector<AST*>(std::begin(params), std::end(params)); }

VariableType NewExpressionNode::getType() const { return call_info.callee -> function_type_info.return_type; }
bool NewExpressionNode::isLeftValue() const { return false; }
