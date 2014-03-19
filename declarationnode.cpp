#include "declarationnode.hpp"

DeclarationNode::~DeclarationNode() { }

void DeclarationNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> template_expr) { GlobalHelper::setDefined(getDefinedSymbol()); }
