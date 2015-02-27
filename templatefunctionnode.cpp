#include "templatefunctionnode.hpp"
#include "functionalsymbol.hpp"

TemplateFunctionNode::TemplateFunctionNode(std::string name, TemplateArgumentsInfo template_arguments) : name_(name)
                                                                                                       , template_arguments(template_arguments)
{

}

ASTNode TemplateFunctionNode::copyTree() const { return std::make_unique<TemplateFunctionNode>(name_, template_arguments); }

VariableType TemplateFunctionNode::getType() const { return function_; }
bool TemplateFunctionNode::isLeftValue() const { return false; }

bool TemplateFunctionNode::isCompileTimeExpr() const { return false; }
boost::optional<int> TemplateFunctionNode::getCompileTimeValue() const { return boost::none; }

std::string TemplateFunctionNode::toString() const 
{
    auto result = name_;
    if ( !template_arguments.empty() ) {
        result += ::toString(template_arguments);
    }
    return result;
}

void TemplateFunctionNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
const std::string& TemplateFunctionNode::name() const { return name_; }

FunctionalSymbol* TemplateFunctionNode::function() { return function_; }
void TemplateFunctionNode::function(FunctionalSymbol* type) { function_ = type; }

const TemplateArgumentsInfo& TemplateFunctionNode::templateArgumentsInfo() const { return template_arguments; }
