#include "functionnode.hpp"
#include "functionalsymbol.hpp"

FunctionNode::FunctionNode(const std::string& name, ModuleSymbol* sym) : name_(name), module_(sym) { }

ASTNode FunctionNode::copyTree() const { return std::make_unique<FunctionNode>(name_); }

VariableType FunctionNode::getType() const { return function_; }
bool FunctionNode::isLeftValue() const { return false; }

bool FunctionNode::isCompileTimeExpr() const { return false; }
boost::optional<int> FunctionNode::getCompileTimeValue() const { return boost::none; }

std::string FunctionNode::toString() const { return name_; }
void FunctionNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

const std::string& FunctionNode::name() const { return name_; }

FunctionalSymbol* FunctionNode::function() { return function_; }
void FunctionNode::function(FunctionalSymbol* sym) { function_ = sym; }

ModuleSymbol* FunctionNode::module() const { return module_; }
