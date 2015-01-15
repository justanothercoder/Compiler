#include "externnode.hpp"
#include "functionsymbol.hpp"

ExternNode::ExternNode(const std::string& name, FunctionDeclarationInfo info, bool is_unsafe) : name_    (name)
                                                                                              , info_    (info)
                                                                                              , is_unsafe(is_unsafe)
{

}

ASTNode ExternNode::copyTree() const { return std::make_unique<ExternNode>(name_, info_, is_unsafe); }
std::string ExternNode::toString() const { return "extern " + name_ + " " + info_.toString(); }

void ExternNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
const Symbol* ExternNode::getDefinedSymbol() const { return defined_symbol.get(); }
void ExternNode::setDefinedSymbol(std::shared_ptr<const FunctionSymbol> sym) { defined_symbol = sym; }
    
const std::string& ExternNode::name() const { return name_; }
const FunctionDeclarationInfo& ExternNode::info() const { return info_; }

bool ExternNode::isUnsafe() const { return is_unsafe; }
