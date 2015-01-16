#include "templatefunctionnode.hpp"
#include "functionaltype.hpp"

TemplateFunctionNode::TemplateFunctionNode(const std::string& name, std::vector<TemplateParamInfo> template_params) : name_(name)
                                                                                                                    , template_params(template_params)
{

}

ASTNode TemplateFunctionNode::copyTree() const { return std::make_unique<TemplateFunctionNode>(name_, template_params); }

VariableType TemplateFunctionNode::getType() const { return function_; }
bool TemplateFunctionNode::isLeftValue() const { return false; }

bool TemplateFunctionNode::isCompileTimeExpr() const { return false; }
boost::optional<int> TemplateFunctionNode::getCompileTimeValue() const { return boost::none; }

std::string TemplateFunctionNode::toString() const 
{
    auto result = name_;
    if ( !template_params.empty() )
    {
        result += "<";
        
        struct StringifyVisitor : boost::static_visitor<std::string>
        {
            auto operator()(const std::shared_ptr<ExprNode>& expr) { return expr -> toString(); }
            auto operator()(const TypeInfo& type_info) { return type_info.toString(); }
        } stringify;

        auto it = std::begin(template_params);

        result += boost::apply_visitor(stringify, *it);
        for ( ++it; it != std::end(template_params); ++it )
            result += ", " + boost::apply_visitor(stringify, *it);

        result += ">";
    }
    return result;
}

void TemplateFunctionNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
const std::string& TemplateFunctionNode::name() const { return name_; }

const FunctionalType* TemplateFunctionNode::function() { return function_; }
void TemplateFunctionNode::function(const FunctionalType* type) { function_ = type; }

const std::vector<TemplateParamInfo>& TemplateFunctionNode::templateParams() const { return template_params; }
