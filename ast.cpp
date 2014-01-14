#include "ast.hpp"

AST::~AST() { }

Scope* AST::getScope() const { return GlobalHelper::getASTScope(this); }
void AST::setScope(Scope *sc) { return GlobalHelper::setASTScope(this, sc); }
