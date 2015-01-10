#include "callnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

CallNode::CallNode(ExprNode *caller, std::vector<ExprNode*> params) : caller(caller), params(params)
{

}

AST* CallNode::copyTree() const
{
    std::vector<ExprNode*> expr;
    for ( auto param : params )
        expr.push_back(static_cast<ExprNode*>(param -> copyTree()));

    return new CallNode(static_cast<ExprNode*>(caller -> copyTree()), expr);
}

std::vector<AST*> CallNode::getChildren() const
{
    auto vec = std::vector<AST*>{ caller };
    vec.insert(std::begin(vec), std::begin(params), std::end(params));
    return vec;
}

VariableType CallNode::getType() const { return call_info.callee -> type().returnType(); }
bool CallNode::isLeftValue() const { return false; }

bool CallNode::isCompileTimeExpr() const
{
    return call_info.callee -> is_constexpr && std::all_of(std::begin(params), std::end(params), [](auto expr)
    {
        return expr -> isCompileTimeExpr();
    });
}

boost::optional<int> CallNode::getCompileTimeValue() const { return boost::none; }

std::string CallNode::toString() const
{
    auto res = caller -> toString();

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

void CallNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
