#include "numbernode.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"

NumberNode::NumberNode(const std::string& num) : num_(num) { }

ASTNode NumberNode::copyTree() const { return std::make_unique<NumberNode>(num_); }

VariableType NumberNode::getType() const { return VariableType(BuiltIns::int_type.get(), true); }
bool NumberNode::isLeftValue() const { return false; }

bool NumberNode::isCompileTimeExpr() const { return true; }
boost::optional<int> NumberNode::getCompileTimeValue() const { return boost::optional<int>(std::stoi(num_)); }

std::string NumberNode::toString() const { return num_; }
void NumberNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
std::string NumberNode::num() const { return num_; }
