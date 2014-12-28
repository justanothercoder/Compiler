#include "externnode.hpp"
#include "functionsymbol.hpp"

ExternNode::ExternNode(std::string name, FunctionDeclarationInfo info, bool is_unsafe) : name         (name)
                                                                                       , info         (info)
                                                                                       , is_unsafe    (is_unsafe)
                                                                                       , definedSymbol(nullptr)
{

}

AST* ExternNode::copyTree() const { return new ExternNode(name, info, is_unsafe); }
std::string ExternNode::toString() const { return "extern " + name + " " + info.toString(); }

void ExternNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
Symbol* ExternNode::getDefinedSymbol() const { return definedSymbol; }
