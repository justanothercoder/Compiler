#include "functiondeclarationnode.hpp"

#include "typefactory.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(std::string name
                                               , FunctionDeclarationInfo info
                                               , AST *statements
                                               , FunctionTraits traits
                                               , bool is_unsafe
                                               , boost::optional<TemplateInfo> template_info) : name         (name)
                                                                                              , info         (info)
                                                                                              , statements   (statements)
                                                                                              , traits       (traits)
                                                                                              , definedSymbol(nullptr)
                                                                                              , func_scope   (nullptr)
                                                                                              , is_unsafe    (is_unsafe)
                                                                                              , template_info(template_info)
{

}

void FunctionDeclarationNode::build_scope()
{
    std::string function_name = (traits.is_operator ? Comp::config().getCodeOperatorName(name) : name);
    std::string scope_name = scope -> getScopeName() + "_" + function_name;

    func_scope = new FunctionScope(scope_name, scope, is_unsafe);
    func_scope -> template_info = (!template_info ? scope -> templateInfo() : *template_info);

    statements -> scope = func_scope;
    statements -> build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

AST* FunctionDeclarationNode::copyTree() const { return new FunctionDeclarationNode(name, info, statements -> copyTree(), traits); }

std::vector<AST*> FunctionDeclarationNode::getChildren() const { return {statements}; }

std::string FunctionDeclarationNode::toString() const
{
    std::string res = "";
    
    res += "def " + name;
    res += info.toString();
    res += statements -> toString();

    return res;
    return "def " + name + " " + info.toString() + " " + statements -> toString();
}

void FunctionDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
