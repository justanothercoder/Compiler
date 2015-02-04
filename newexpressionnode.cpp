#include "newexpressionnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, std::vector<ASTExprNode> params) : type_info(type_info), params_(std::move(params)) { }

void NewExpressionNode::build_scope()
{
    AST::build_scope();
    for ( auto param : type_info.templateArgumentsInfo() )
    {
        if ( param.which() == 0 )
        {
            boost::get< std::shared_ptr<ExprNode> >(param) -> scope = scope;
            boost::get< std::shared_ptr<ExprNode> >(param) -> build_scope();
        }
    }
    
    for ( const auto& modifier : type_info.modifiers() )
    {
        if ( modifier.isDimension() )
        {
            auto dim = *modifier.dimension();
            dim -> scope = scope;
            dim -> build_scope();
        }
    }   
}

ASTNode NewExpressionNode::copyTree() const
{
    auto vec = std::vector<ASTExprNode>{ };
    for ( const auto& param : params_ )
        vec.push_back(ASTExprNode(static_cast<ExprNode*>(param -> copyTree().release())));

    return std::make_unique<NewExpressionNode>(type_info, std::move(vec));
}

ASTChildren NewExpressionNode::getChildren() const 
{
    auto children = ASTChildren{ };

    for ( const auto& param : params_ )
        children.push_back(param.get());

    return children;
}

VariableType NewExpressionNode::getType() const { return VariableType(call_info.callee -> type().returnType().unqualified(), false); }

bool NewExpressionNode::isLeftValue() const       { return false; } 
bool NewExpressionNode::isCompileTimeExpr() const { return false; }
boost::optional<int> NewExpressionNode::getCompileTimeValue() const { return boost::none; }

std::string NewExpressionNode::toString() const
{
    auto res = "new " + type_info.toString();
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

void NewExpressionNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

const TypeInfo& NewExpressionNode::typeInfo() const { return type_info; }
void NewExpressionNode::typeInfo(const TypeInfo& type_info) { this -> type_info = type_info; }

const std::vector<ASTExprNode>& NewExpressionNode::params() const { return params_; }

