#include "dotnode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

DotNode::DotNode(ExprNode *base, std::string member_name) : base(base), member_name(member_name), base_type(nullptr), member(nullptr)
{

}

std::vector<AST*> DotNode::getChildren() const
{
    return {base};
}

AST* DotNode::copyTree() const
{
    return new DotNode(static_cast<ExprNode*>(base -> copyTree()), member_name);
}

VariableType DotNode::getType() const
{
    return member -> getSymbolType() == SymbolType::VARIABLE ? static_cast<VariableSymbol*>(member) -> getType() 
                                                             : VariableType(static_cast<OverloadedFunctionSymbol*>(member), true);
}

bool DotNode::isLeftValue() const
{
    return true;
}

bool DotNode::isCompileTimeExpr() const
{
    return false;
}

boost::optional<int> DotNode::getCompileTimeValue() const
{
    return boost::none;
}

std::string DotNode::toString() const
{
    return base -> toString() + "." + member_name;
}

void DotNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
