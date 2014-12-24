#include "variabledeclarationnode.hpp"
#include "callhelper.hpp"
#include "exprnode.hpp"
#include "scope.hpp"
#include "templatestructsymbol.hpp"
#include "builtins.hpp"

VariableDeclarationNode::VariableDeclarationNode(std::string name
                                               , TypeInfo type_info
                                               , bool is_field
                                               , std::vector<ExprNode*> constructor_params) : name              (name)
                                                                                            , type_info         (type_info)
                                                                                            , is_field          (is_field)
                                                                                            , definedSymbol     (nullptr)
                                                                                            , constructor_params(constructor_params)
                                                                                            , inline_call_body  (nullptr)                                                                                              
{

}

void VariableDeclarationNode::build_scope()
{
    AST::build_scope();
    for ( auto param : type_info.template_params )
    {
        if ( param.which() == 0 )
        {
            boost::get<ExprNode*>(param) -> scope = scope;
            boost::get<ExprNode*>(param) -> build_scope();
        }
    }
}

Symbol* VariableDeclarationNode::getDefinedSymbol() const
{
    return definedSymbol;
}

AST* VariableDeclarationNode::copyTree() const
{
    std::vector<ExprNode*> params;

    for ( auto param : constructor_params )
        params.push_back(static_cast<ExprNode*>(param -> copyTree()));

    return new VariableDeclarationNode(name, type_info, is_field, params);
}

std::vector<AST*> VariableDeclarationNode::getChildren() const
{
    return std::vector<AST*>(std::begin(constructor_params), std::end(constructor_params));
}

std::string VariableDeclarationNode::toString() const
{
    return type_info.toString() + " " + name + ";";
}

void VariableDeclarationNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
