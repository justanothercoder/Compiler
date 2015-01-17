#include "varinfertypedeclarationnode.hpp"
#include "scope.hpp"
#include "exprnode.hpp"
#include "variablesymbol.hpp"

VarInferTypeDeclarationNode::VarInferTypeDeclarationNode(const std::string& name, ASTExprNode expr) : name_(name), expr_(std::move(expr)) { }

void VarInferTypeDeclarationNode::build_scope()
{
    expr_ -> scope = scope;
    expr_ -> build_scope();
}

const Symbol* VarInferTypeDeclarationNode::getDefinedSymbol() const { return defined_symbol.get(); }

ASTNode VarInferTypeDeclarationNode::copyTree() const 
{ 
    return std::make_unique<VarInferTypeDeclarationNode>(name_, ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release())));
}

std::vector<AST*> VarInferTypeDeclarationNode::getChildren() const { return {expr_.get()}; }
std::string VarInferTypeDeclarationNode::toString() const { return "var " + name_ + " = " + expr_ -> toString() + ";"; }

void VarInferTypeDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

void VarInferTypeDeclarationNode::setDefinedSymbol(std::shared_ptr<const VariableSymbol> sym) { defined_symbol = sym; }
    
const std::string& VarInferTypeDeclarationNode::name() const { return name_; }
ExprNode* VarInferTypeDeclarationNode::expr() { return expr_.get(); }

const CallInfo& VarInferTypeDeclarationNode::callInfo() const { return call_info; }
void VarInferTypeDeclarationNode::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }

const InlineInfo& VarInferTypeDeclarationNode::inlineInfo() const { return inline_info; }
void VarInferTypeDeclarationNode::inlineInfo(InlineInfo inline_info) { this -> inline_info = std::move(inline_info); }
