#include "callnode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "globaltable.hpp"

CallNode::CallNode(ExprNode *caller, std::vector<ExprNode*> params) : caller(caller), params(params) 
{

}

AST* CallNode::copyTree() const
{
	std::vector<ExprNode*> expr(params.size());
    std::transform(std::begin(params), std::end(params), std::begin(expr), [&] (ExprNode *ex) 
	{ 
		return static_cast<ExprNode*>(ex -> copyTree()); 
	});

    return new CallNode(static_cast<ExprNode*>(caller -> copyTree()), expr);
}

std::vector<AST*> CallNode::getChildren() const
{
	std::vector<AST*> vec { caller };
	vec.insert(std::begin(vec), std::begin(params), std::end(params));
	return vec;
}

const Type* CallNode::getType() const 
{
   	return call_info.callee -> return_type; 
}

bool CallNode::isLeftValue() const 
{
   	return false; 
}

void CallNode::freeTempSpace()
{

}

bool CallNode::isCompileTimeExpr() const
{
	return call_info.callee -> is_constexpr && std::all_of(std::begin(params), std::end(params), [&](ExprNode *expr) 
	{ 
		return expr -> isCompileTimeExpr(); 
	});
}

boost::optional<int> CallNode::getCompileTimeValue() const
{
	return boost::none;
}
	
std::string CallNode::toString() const 
{
	std::string res = caller -> toString();
   
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

void CallNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
