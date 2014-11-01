#include "varinfertypedeclarationnode.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "callhelper.hpp"
#include "variablesymboldefine.hpp"
#include "globaltable.hpp"
#include "exprnode.hpp"

VarInferTypeDeclarationNode::VarInferTypeDeclarationNode(std::string name
                                                       , ExprNode *expr) : name(name)
                                                                         , expr(expr)
                                                                         , definedSymbol(nullptr)
{

}

Symbol* VarInferTypeDeclarationNode::getDefinedSymbol() const
{
    return definedSymbol;
}

AST* VarInferTypeDeclarationNode::copyTree() const
{
    return new VarInferTypeDeclarationNode(name, static_cast<ExprNode*>(expr -> copyTree()));
}

std::vector<AST*> VarInferTypeDeclarationNode::getChildren() const
{
    return {expr};
}

std::string VarInferTypeDeclarationNode::toString() const
{
    return "var " + name + " = " + expr -> toString() + ";";
}

void VarInferTypeDeclarationNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
