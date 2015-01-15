#include "typenode.hpp"
#include "symbol.hpp"

TypeNode::TypeNode(const std::string& name) : name_(name) { }

ASTNode TypeNode::copyTree() const { return std::make_unique<TypeNode>(name_); }

VariableType TypeNode::getType() const { return nullptr; } 
bool TypeNode::isLeftValue() const { return false; }

bool TypeNode::isCompileTimeExpr() const { return false; } 
boost::optional<int> TypeNode::getCompileTimeValue() const { return boost::none; }

std::string TypeNode::toString() const { return name_; }
void TypeNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
const std::string& TypeNode::name() const { return name_; }

const Symbol* TypeNode::typeSymbol() const { return type_symbol; }
void TypeNode::typeSymbol(const Symbol* sym) { type_symbol = sym; }
