#include "stringnode.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"

StringNode::StringNode(const std::string& str) : str_(str) { }

ASTNode StringNode::copyTree() const { return std::make_unique<StringNode>(str_); }
std::string StringNode::str() const { return str_; }

VariableType StringNode::getType() const { return VariableType(BuiltIns::ASCII_string_type.get(), true); }
bool StringNode::isLeftValue() const { return false; }

bool StringNode::isCompileTimeExpr() const { return false; } 
boost::optional<int> StringNode::getCompileTimeValue() const { return boost::none; }

std::string StringNode::toString() const { return '"' + str_ + '"'; }
void StringNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
