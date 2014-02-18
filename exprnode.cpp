#include "exprnode.hpp"

void ExprNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }

bool ExprNode::isLeftValue() const { return getType()->isReference(); }
