#include "newexpressionnode.hpp"
#include "functionsymbol.hpp"
#include "callhelper.hpp"
#include "structsymbol.hpp"
#include "globaltable.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, std::vector<ExprNode*> params) : type_info(type_info), params(params)
{

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

AST* NewExpressionNode::copyTree() const
{
    std::vector<ExprNode*> vec(params.size());
    std::transform(std::begin(params), std::end(params), std::begin(vec), [](ExprNode *e)
    {
        return static_cast<ExprNode*>(e -> copyTree());
    });

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
