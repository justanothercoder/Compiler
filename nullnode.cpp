#include "nullnode.hpp"
#include "scope.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"

ASTNode NullNode::copyTree() const { return std::make_unique<NullNode>(); }

std::string NullNode::toString() const { return "null"; }
VariableType NullNode::getType() const { return VariableType(TypeFactory::getPointer(BuiltIns::void_type), true); }

bool NullNode::isLeftValue() const       { return false; } 
bool NullNode::isCompileTimeExpr() const { return false; }

boost::optional<int> NullNode::getCompileTimeValue() const { return boost::none; } 
void NullNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
