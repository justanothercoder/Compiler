#include "variablenode.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

VariableNode::VariableNode(const std::string& name) : name_(name) { }
ASTNode VariableNode::copyTree() const { return std::make_unique<VariableNode>(name_); }

VariableType VariableNode::getType() const { return variable_ -> typeOf(); }
bool VariableNode::isLeftValue() const { return true; } 

bool VariableNode::isCompileTimeExpr() const { return false; }
boost::optional<int> VariableNode::getCompileTimeValue() const { return boost::none; }

std::string VariableNode::toString() const { return name_; }
void VariableNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
const std::string& VariableNode::name() const { return name_; }

const VarSymbol* VariableNode::variable() const { return variable_; }
void VariableNode::variable(const VarSymbol* sym) { variable_ = sym; }
