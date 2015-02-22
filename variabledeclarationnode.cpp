#include "variabledeclarationnode.hpp"
#include "exprnode.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "variablesymbol.hpp"
#include "templatestructsymbol.hpp"

#include "logger.hpp"

VariableDeclarationNode::VariableDeclarationNode(const std::string& name
                                               , TypeInfo type_info
                                               , bool is_field
                                               , std::vector<ASTExprNode> constructor_params) : name_             (name)
                                                                                              , type_info         (type_info)
                                                                                              , is_field          (is_field)
                                                                                              , constructor_params(std::move(constructor_params))
{

}

void VariableDeclarationNode::build_scope()
{
    AST::build_scope();

    struct BuildScopeVisitor : boost::static_visitor<>
    {
        BuildScopeVisitor(const std::shared_ptr<Scope>& scope) : scope(scope) { }

        auto operator()(const std::shared_ptr<ExprNode>& expr) { expr -> scope = scope; expr -> build_scope(); }
        auto operator()(const TypeInfo&) { }

        const std::shared_ptr<Scope>& scope;
    } build(scope);

    for ( auto param : type_info.templateArgumentsInfo() )
        boost::apply_visitor(build, param);

    for ( const auto& modifier : type_info.modifiers() )
    {
        if ( modifier.isDimension() )
        {
            auto dim = *modifier.dimension();
            dim -> scope = scope;
            dim -> build_scope();
        }
    }        
}

Symbol* VariableDeclarationNode::getDefinedSymbol() const { return defined_symbol; }

ASTNode VariableDeclarationNode::copyTree() const
{
    auto params = std::vector<ASTExprNode>{ };

    for ( const auto& param : constructor_params )
    {
        auto copy = param -> copyTree();
        params.push_back(ASTExprNode(static_cast<ExprNode*>(copy.release())));
    }

    return std::make_unique<VariableDeclarationNode>(name_, type_info, is_field, std::move(params));
}

ASTChildren VariableDeclarationNode::getChildren() const 
{
    auto children = ASTChildren{ };

    for ( const auto& param : constructor_params )
        children.push_back(param.get());

    return children;
}

std::string VariableDeclarationNode::toString() const 
{
    auto result = std::string("");
    result += type_info.toString() + " " + name_;

    if ( !constructor_params.empty() )
    {
        result += "(";
        
        auto it = std::begin(constructor_params);
        result += (*it) -> toString();
        
        for ( ++it; it != std::end(constructor_params); ++it )
            result += ", " + (*it) -> toString();                
    
        result += ")";
    }

    return result;
}

void VariableDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

void VariableDeclarationNode::setDefinedSymbol(VarSymbol* sym) { defined_symbol = sym; }
    
const std::string& VariableDeclarationNode::name() const { return name_; }

bool VariableDeclarationNode::isField() const { return is_field; } 
const std::vector<ASTExprNode>& VariableDeclarationNode::constructorParams() const { return constructor_params; }

const TypeInfo& VariableDeclarationNode::typeInfo() const { return type_info; }
void VariableDeclarationNode::typeInfo(const TypeInfo& type_info) { this -> type_info = type_info; }
