#include "functiondeclarationnode.hpp"

#include "typefactory.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(std::string name
                                               , FunctionDeclarationInfo info
                                               , AST *statements
                                               , FunctionTraits traits
                                               , bool is_unsafe) : name         (name)
                                                                 , info         (info)
                                                                 , statements   (statements)
                                                                 , traits       (traits)
                                                                 , definedSymbol(nullptr)
                                                                 , func_scope   (nullptr)
                                                                 , is_unsafe    (is_unsafe)
{

}

void FunctionDeclarationNode::build_scope()
{
    std::string function_name = (traits.is_operator ? GlobalConfig::getCodeOperatorName(name) : name);

    func_scope = new FunctionScope(scope -> getScopeName() + "_" + function_name, scope, traits.is_constructor, is_unsafe);

    statements -> scope = func_scope;
    statements -> build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const
{
    return definedSymbol;
}

AST* FunctionDeclarationNode::copyTree() const
{
    return new FunctionDeclarationNode(name, info, statements -> copyTree(), traits);
}

std::vector<AST*> FunctionDeclarationNode::getChildren() const
{
    return {statements};
}

std::string FunctionDeclarationNode::toString() const
{
    std::string res = info.returnTypeInfo().toString() + " " + name + "(";

    if ( !info.formalParams().empty() )
    {
        const auto& params = info.formalParams();

        auto it = std::begin(params);

        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(params); ++it )
            res += ", " + it -> second.toString() + " " + it -> first;
    }

    res += ")";
    res += statements -> toString();

    return res;
}

void FunctionDeclarationNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
