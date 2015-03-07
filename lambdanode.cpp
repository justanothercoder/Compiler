#include "lambdanode.hpp"
#include "functionalsymbol.hpp"
#include "objecttype.hpp"

LambdaNode::LambdaNode(std::vector<std::string> capture, std::vector<ParamInfo> formal_params, ASTNode body) : capture_     (capture)
                                                                                                             , formal_params(formal_params)
                                                                                                             , body_        (std::move(body))
{

}
    
void LambdaNode::build_scope() 
{
    body_ -> scope = scope;
    body_ -> build_scope();    
}

ASTNode LambdaNode::copyTree() const { return std::make_unique<LambdaNode>(capture_, formal_params, body_ -> copyTree()); }

std::string LambdaNode::toString() const
{
    std::string res = "";
    res += "[";

    if ( !capture_.empty() )
    {
        auto it = std::begin(capture_);
        res += *it;

        for ( ++it; it != std::end(capture_); ++it )
            res += ", " + *it;
    }

    res += "]";
    res += "(";

    if ( !formal_params.empty() )
    {
        auto it = std::begin(formal_params);
        res += it -> toString();

        for ( ++it; it != std::end(formal_params); ++it )
            res += ", " + it -> toString();
    }

    res += ")";
    res += body_ -> toString();

    return res;
}

void LambdaNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

VariableType LambdaNode::getType() const { return VariableType(lambda_inner_type, true); }
bool LambdaNode::isLeftValue() const { return false; }

bool LambdaNode::isCompileTimeExpr() const { return false; }
boost::optional<int> LambdaNode::getCompileTimeValue() const { return boost::none; }

const std::vector<std::string>& LambdaNode::capture() const { return capture_; }
const std::vector<ParamInfo> LambdaNode::formalParams() const { return formal_params; }
AST* LambdaNode::body() const { return body_.get(); }
   
void LambdaNode::setLambdaType(Type* t) { lambda_inner_type = t; }
    
const FunctionalSymbol* LambdaNode::callOp() const 
{
    auto methods = static_cast<const ObjectType*>(lambda_inner_type) -> methods();
    for ( auto meth : methods )
    {
        if ( meth -> getName() == "operator()" )
            return meth;
    }

    return nullptr;

//    auto function = static_cast<const ObjectType*>(lambda_inner_type) -> resolveMethod("operator()", {/*TODO add args*/});
//    return function;
}
