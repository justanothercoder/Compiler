#include "newexpressionnode.hpp"
#include "functionsymbol.hpp"
#include "callhelper.hpp"
#include "structsymbol.hpp"
#include "globalhelper.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, std::vector<ExprNode*> params) : type_info(type_info), params(params)
{

}

NewExpressionNode::~NewExpressionNode()
{
	for ( auto i : params )
		delete i;
}

void NewExpressionNode::build_scope()
{
	AST::build_scope();
	for ( auto param : type_info.template_params )
	{
		if ( param.which() == 0 )
		{
			boost::get<ExprNode*>(param) -> scope = scope;
			boost::get<ExprNode*>(param) -> build_scope();
		}
	}
}

CodeObject& NewExpressionNode::gen()
{
    auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
   	scope -> getTempAlloc().claim(getType() -> getSize());

	auto addr2 = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(GlobalConfig::int_size);

	CodeObject new_place;
	new_place.emit("lea rax, " + addr);

	code_obj.genCallCode(call_info, params, new_place, false);

	code_obj.emit("lea rax, " + addr);
	code_obj.emit("mov " + addr2 + ", rax");
	code_obj.emit("lea rax, " + addr2);

	return code_obj;
}

AST* NewExpressionNode::copyTree() const 
{
	std::vector<ExprNode*> vec(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(vec), [](ExprNode *e) { return static_cast<ExprNode*>(e -> copyTree()); });

	return new NewExpressionNode(type_info, vec);
}

std::vector<AST*> NewExpressionNode::getChildren() const 
{ 
	return std::vector<AST*>(std::begin(params), std::end(params)); 
}

const Type* NewExpressionNode::getType() const 
{
   	return call_info.callee -> return_type; 
}

bool NewExpressionNode::isLeftValue() const 
{
   	return false; 
}

void NewExpressionNode::freeTempSpace()
{
	scope -> getTempAlloc().free();	
}

bool NewExpressionNode::isCompileTimeExpr() const
{
	return false;
}

boost::optional<int> NewExpressionNode::getCompileTimeValue() const
{
	return boost::none;
}
	
std::string NewExpressionNode::toString() const 
{
	std::string res = "new " + type_info.toString();

	res += "(";

	if ( !params.empty() )
	{
		auto it = std::begin(params);
		res += (*it) -> toString();

		for ( ++it; it != std::end(params); ++it )
			res += ", " + (*it) -> toString();

	}
	res += ")";
	
	return res;
}
	
void NewExpressionNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
