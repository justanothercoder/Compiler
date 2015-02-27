#include "varinfertypedeclarationnode.hpp"
#include "scope.hpp"
#include "exprnode.hpp"
#include "varsymbol.hpp"
#include "functionalsymbol.hpp"

VarInferTypeDeclarationNode::VarInferTypeDeclarationNode(std::string name, ASTExprNode expr) : name_(name), expr_(std::move(expr)) { }

void VarInferTypeDeclarationNode::build_scope()
{
    expr_ -> scope = scope;
    expr_ -> build_scope();
}

Symbol* VarInferTypeDeclarationNode::getDefinedSymbol() const { return defined_symbol; }

ASTNode VarInferTypeDeclarationNode::copyTree() const 
{ 
    return std::make_unique<VarInferTypeDeclarationNode>(name_, ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release())));
}

std::vector<AST*> VarInferTypeDeclarationNode::getChildren() const { return {expr_.get()}; }
std::string VarInferTypeDeclarationNode::toString() const { return "var " + name_ + " = " + expr_ -> toString() + ";"; }

void VarInferTypeDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

void VarInferTypeDeclarationNode::setDefinedSymbol(VarSymbol* sym) { defined_symbol = sym; }
    
const std::string& VarInferTypeDeclarationNode::name() const { return name_; }
ExprNode* VarInferTypeDeclarationNode::expr() { return expr_.get(); }
