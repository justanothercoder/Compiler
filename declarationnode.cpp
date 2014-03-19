#include "declarationnode.hpp"

DeclarationNode::~DeclarationNode() { }

void DeclarationNode::check(const TemplateInfo&) { GlobalHelper::setDefined(getDefinedSymbol()); }
