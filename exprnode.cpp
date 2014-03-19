#include "exprnode.hpp"

void ExprNode::define(const TemplateInfo&) { }

bool ExprNode::isLeftValue() const { return getType().is_ref; }
