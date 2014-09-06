#include "newexpressionnode.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
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
		param -> scope = scope;
		param -> build_scope();
	}
}

void NewExpressionNode::check()
{
	for ( auto param : type_info.template_params )
		param -> check();

	auto type = static_cast<const StructSymbol*>(TypeHelper::fromTypeInfo(type_info, scope, scope -> getTemplateInfo()));

	call_info = CallHelper::callCheck(type -> getName(), type, params); 

	scope -> getTempAlloc().add(type -> getSize());      //place for object itself
	scope -> getTempAlloc().add(GlobalConfig::int_size); //place for reference to it
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
