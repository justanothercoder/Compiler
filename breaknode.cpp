#include "breaknode.hpp"

ASTNode BreakNode::copyTree() const { return std::make_unique<BreakNode>(); }

std::string BreakNode::toString() const { return "break"; }
void BreakNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
