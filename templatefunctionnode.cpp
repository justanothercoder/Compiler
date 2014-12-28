#include "templatefunctionnode.hpp"

TemplateFunctionNode::TemplateFunctionNode(std::string name, std::vector<TemplateParamInfo> template_params) : name(name)
                                                                                                             , template_params(template_params)
                                                                                                             , function(nullptr)
{

}

AST* TemplateFunctionNode::copyTree() const { return new TemplateFunctionNode(name, template_params); }

VariableType TemplateFunctionNode::getType() const { return function; }
bool TemplateFunctionNode::isLeftValue() const { return false; }

bool TemplateFunctionNode::isCompileTimeExpr() const { return false; }
boost::optional<int> TemplateFunctionNode::getCompileTimeValue() const { return boost::none; }

std::string TemplateFunctionNode::toString() const { return name; }

void TemplateFunctionNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
