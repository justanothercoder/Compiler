#include "callnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

CallNode::CallNode(ASTExprNode caller, std::vector<ASTExprNode> params) : caller_(std::move(caller)), params_(std::move(params)) { }

ASTNode CallNode::copyTree() const
{
    auto expr = std::vector<ASTExprNode>{ };
    for ( const auto& param : params_ )
    {
        auto copy = param -> copyTree();
        expr.push_back(ASTExprNode(static_cast<ExprNode*>(copy.release())));
    }

    return std::make_unique<CallNode>(ASTExprNode(static_cast<ExprNode*>(caller_ -> copyTree().release())), std::move(expr));
}

ASTChildren CallNode::getChildren() const
{
    auto vec = ASTChildren{ caller_.get() };
    for ( const auto& param : params_ )
        vec.push_back(param.get());

    return vec;
}

bool CallNode::isCompileTimeExpr() const
{
    return call_info.callee -> is_constexpr && std::all_of(std::begin(params_), std::end(params_), [](auto&& expr)
    {
        return expr -> isCompileTimeExpr();
    });
}

boost::optional<int> CallNode::getCompileTimeValue() const { return boost::none; }

std::string CallNode::toString() const
{
    auto res = caller_ -> toString();

    res += "(";

    if ( !params_.empty() )
    {
        auto it = std::begin(params_);
        res += (*it) -> toString();

        for ( ++it; it != std::end(params_); ++it )
            res += ", " + (*it) -> toString();
    }

    res += ")";

    return res;
}

void CallNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

ExprNode* CallNode::caller() { return caller_.get(); }
const std::vector<ASTExprNode>& CallNode::params() const { return params_; }
    
const FunctionalType* CallNode::function() const
{
    if ( caller_ -> getType().unqualified() -> isObjectType() )
        return static_cast<const ObjectType*>(caller_ -> getType().unqualified()) -> resolveMethod("operator()");
    else
        return static_cast<const FunctionalType*>(caller_ -> getType().unqualified());
}

std::vector<ValueInfo> CallNode::arguments() const
{
    auto result = std::vector<ValueInfo>{ };

    for ( const auto& param : params_ )
        result.emplace_back(param -> getType(), param -> isLeftValue());

    return result;
}
