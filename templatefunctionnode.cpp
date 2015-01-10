#include "templatefunctionnode.hpp"
#include "functionaltype.hpp"

TemplateFunctionNode::TemplateFunctionNode(std::string name, std::vector<TemplateParamInfo> template_params) : name(name)
                                                                                                             , function(nullptr)
                                                                                                             , template_params(template_params)
{

}

AST* TemplateFunctionNode::copyTree() const { return new TemplateFunctionNode(name, template_params); }

VariableType TemplateFunctionNode::getType() const { return function; }
bool TemplateFunctionNode::isLeftValue() const { return false; }

bool TemplateFunctionNode::isCompileTimeExpr() const { return false; }
boost::optional<int> TemplateFunctionNode::getCompileTimeValue() const { return boost::none; }

std::string TemplateFunctionNode::toString() const 
{
    std::string result = name;
    if ( !template_params.empty() )
    {
        result += "<";

        auto it = std::begin(template_params);
        if ( it -> which() == 0 )
            result += boost::get<ExprNode*>(*it) -> toString();
        else
            result += boost::get<TypeInfo>(*it).toString();

        for ( ++it; it != std::end(template_params); ++it )
        {
            if ( it -> which() == 0 )
                result += boost::get<ExprNode*>(*it) -> toString();
            else
                result += boost::get<TypeInfo>(*it).toString();
        }

        result += ">";
    }
    return result;
}

void TemplateFunctionNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
