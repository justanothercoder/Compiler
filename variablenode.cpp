#include "variablenode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"

VariableNode::VariableNode(std::string name) : name(name), variable(nullptr), template_num(nullptr) { }

bool VariableNode::isTemplateParam() const
{
    const auto& template_info = scope -> templateInfo();
    return template_info.sym && template_info.isIn(name);
}

AST* VariableNode::copyTree() const { return new VariableNode(name); }

VariableType VariableNode::getType() const
{
    if ( isTemplateParam() )
    {
        const auto& template_info = scope -> templateInfo();
        auto replace = template_info.getReplacement(name);

        return VariableType(BuiltIns::int_type, true);
    }

    return variable -> getType();
}

bool VariableNode::isLeftValue() const { return true; }

bool VariableNode::isCompileTimeExpr() const
{
    const auto& template_info = scope -> templateInfo();
    return (template_info.sym != nullptr && template_info.isIn(name));
}

boost::optional<int> VariableNode::getCompileTimeValue() const
{
    const auto& template_info = scope -> templateInfo();

    if ( isCompileTimeExpr() )
        return boost::get<int>(*template_info.getReplacement(name));
    else
        return boost::none;
}

std::string VariableNode::toString() const { return name; }

void VariableNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
