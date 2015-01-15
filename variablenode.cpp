#include "variablenode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"

VariableNode::VariableNode(const std::string& name) : name_(name) { }

bool VariableNode::isTemplateParam() const
{
    const auto& template_info = scope -> templateInfo();
    return template_info.sym && template_info.isIn(name_);
}

ASTNode VariableNode::copyTree() const { return std::make_unique<VariableNode>(name_); }

VariableType VariableNode::getType() const
{
    if ( isTemplateParam() )
    {
        const auto& template_info = scope -> templateInfo();
        auto replace = template_info.getReplacement(name_);

        return VariableType(BuiltIns::int_type.get(), true);
    }

    return variable_ -> getType();
}

bool VariableNode::isLeftValue() const { return true; }

bool VariableNode::isCompileTimeExpr() const
{
    const auto& template_info = scope -> templateInfo();
    return (template_info.sym != nullptr && template_info.isIn(name_));
}

boost::optional<int> VariableNode::getCompileTimeValue() const
{
    const auto& template_info = scope -> templateInfo();

    if ( isCompileTimeExpr() )
        return boost::get<int>(*template_info.getReplacement(name_));
    else
        return boost::none;
}

std::string VariableNode::toString() const { return name_; }

void VariableNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
const std::string& VariableNode::name() const { return name_; }
std::unique_ptr<NumberNode>& VariableNode::getNum() { return template_num; }

const VariableSymbol* VariableNode::variable() const { return variable_; }
void VariableNode::variable(const VariableSymbol* sym) { variable_ = sym; }
