#include "functiondeclarationnode.hpp"
#include "typefactory.hpp"
#include "comp.hpp"
#include "globalconfig.hpp"
#include "templatestructsymbol.hpp"
#include "functionalsymbol.hpp"
#include "varsymbol.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(std::string name
                                               , FunctionDeclarationInfo info
                                               , ASTNode statements
                                               , FunctionTraits traits
                                               , bool is_unsafe) : name_        (name)
                                                                 , info_        (info)
                                                                 , statements   (std::move(statements))
                                                                 , traits_      (traits)
                                                                 , is_unsafe    (is_unsafe)
{

}

void FunctionDeclarationNode::build_scope()
{
    auto function_name = (traits_.is_operator ? Comp::config().getCodeOperatorName(name_) : name_);
    auto scope_name = scope -> getScopeName() + "_" + function_name;

    func_scope = std::make_shared<FunctionScope>(scope_name, scope.get(), is_unsafe);

    statements -> scope = func_scope;
    statements -> build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return defined_symbol; } 

ASTNode FunctionDeclarationNode::copyTree() const 
{ 
    return std::make_unique<FunctionDeclarationNode>(name_, info_, statements -> copyTree(), traits_); 
}

ASTChildren FunctionDeclarationNode::getChildren() const { return {statements.get()}; } 
std::string FunctionDeclarationNode::toString() const { return "def " + name_ + info_.toString() + " " + statements -> toString(); }

void FunctionDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

AST* FunctionDeclarationNode::body() { return statements.get(); }
const std::vector<VarSymbol*>& FunctionDeclarationNode::paramsSymbols() const { return params_symbols; }

void FunctionDeclarationNode::addParamSymbol(std::unique_ptr<VarSymbol> var)
{
    params_symbols.push_back(var.get());
    func_scope -> define(std::move(var));
}

FunctionScope* FunctionDeclarationNode::functionScope() const { return func_scope.get(); }
const FunctionTraits& FunctionDeclarationNode::traits() const { return traits_; }

const std::string& FunctionDeclarationNode::name() const { return name_; }

FunctionDeclarationInfo& FunctionDeclarationNode::info()             { return info_; }
const FunctionDeclarationInfo& FunctionDeclarationNode::info() const { return info_; }

void FunctionDeclarationNode::setDefinedSymbol(FunctionalSymbol* symbol) { defined_symbol = symbol; }
bool FunctionDeclarationNode::isUnsafe() const { return is_unsafe; }
