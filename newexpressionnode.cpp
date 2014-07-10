#include "newexpressionnode.hpp"
#include "functionsymbol.hpp"

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

	scope -> getTempAlloc().add(type -> getSize());
}

CodeObject& NewExpressionNode::gen()
{
	string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(getType().type -> getSize());

	CodeObject new_place;
	new_place.emit("lea rax, " + addr);

	code_obj.genCallCode(call_info, params, new_place, false);

	return code_obj;
}

AST* NewExpressionNode::copyTree() const 
{
	vector<ExprNode*> vec(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(vec), [](ExprNode *e) { return static_cast<ExprNode*>(e -> copyTree()); });

	return new NewExpressionNode(type_info, vec);
}

vector<AST*> NewExpressionNode::getChildren() const { return vector<AST*>(std::begin(params), std::end(params)); }

VariableType NewExpressionNode::getType() const { return call_info.callee -> return_type; }
bool NewExpressionNode::isLeftValue() const { return false; }

int NewExpressionNode::neededSpaceForTemporaries()
{
	return getType().getSize();
}

void NewExpressionNode::freeTempSpace()
{
	scope -> getTempAlloc().free();	
}
