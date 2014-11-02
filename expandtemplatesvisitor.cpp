#include "expandtemplatesvisitor.hpp"
#include "templatestructdeclarationnode.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "whilenode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "statementnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "templatestructsymbol.hpp"
#include "variabledeclarationnode.hpp"

void ExpandTemplatesVisitor::visit(IfNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void ExpandTemplatesVisitor::visit(ForNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void ExpandTemplatesVisitor::visit(WhileNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void ExpandTemplatesVisitor::visit(StructDeclarationNode *node)
{
    for ( auto decl : node -> getChildren() )
        decl -> accept(*this);
}

void ExpandTemplatesVisitor::visit(FunctionDeclarationNode *node)
{
    for ( auto i : node -> getChildren() )
        i -> accept(*this);
}

void ExpandTemplatesVisitor::visit(StatementNode *node)
{
    for ( auto i : node -> getChildren() )
        i -> accept(*this);
}

void ExpandTemplatesVisitor::visit(ReturnNode *node)
{
    for ( auto i : node -> getChildren() )
        i -> accept(*this);
}

void ExpandTemplatesVisitor::visit(UnsafeBlockNode *node)
{
    for ( auto i : node -> getChildren() )
        i -> accept(*this);
}

void ExpandTemplatesVisitor::visit(TemplateStructDeclarationNode *node)
{
    node -> scope -> define(node -> definedSymbol);
}

void ExpandTemplatesVisitor::visit(VariableDeclarationNode *node) 
{
    const auto& template_info = node -> scope -> getTemplateInfo();

    if ( template_info.sym && template_info.sym -> isIn(node -> type_info.type_name) )
    {
        auto replace = template_info.getReplacement(node -> type_info.type_name);
        node -> type_info = boost::get<TypeInfo>(*replace);
    }

    auto type = node -> scope -> resolveType(node -> type_info.type_name);

    if ( type == nullptr )
        throw SemanticError(node -> type_info.type_name + " is not a type");
    
    if ( dynamic_cast<const TemplateStructSymbol*>(type) )
    {
        auto& type_info = node -> type_info;

        auto tmpl = dynamic_cast<const TemplateStructSymbol*>(type);

        if ( type_info.template_params.size() != tmpl -> template_symbols.size() )
            throw SemanticError("Wrong number of template parameters");

        auto getTemplateParam = [] ( TemplateParamInfo tp_info )
        {
            if ( tp_info.which() == 1 )
                return TemplateParam(boost::get<TypeInfo>(tp_info));
            else
                return TemplateParam(*boost::get<ExprNode*>(tp_info) -> getCompileTimeValue());
        };

        std::vector<TemplateParam> tmpl_params(type_info.template_params.size());
        std::transform(std::begin(type_info.template_params), std::end(type_info.template_params), std::begin(tmpl_params), getTemplateParam);

        auto decl = getSpecDecl(tmpl, tmpl_params, node -> scope);
        static_cast<TemplateStructDeclarationNode*>(tmpl -> holder) -> instances.insert(decl);
        type_info.type_name = static_cast<StructDeclarationNode*>(decl) -> name;

        decl -> accept(*this);
    }
}

void ExpandTemplatesVisitor::visit(BracketNode *) { }
void ExpandTemplatesVisitor::visit(UnaryNode *) { }
void ExpandTemplatesVisitor::visit(NewExpressionNode *) { }
void ExpandTemplatesVisitor::visit(BinaryOperatorNode *) { }
void ExpandTemplatesVisitor::visit(AddrNode *) { }
void ExpandTemplatesVisitor::visit(NullNode *) { }
void ExpandTemplatesVisitor::visit(DotNode *) { }
void ExpandTemplatesVisitor::visit(VariableNode *) { }
void ExpandTemplatesVisitor::visit(StringNode *) { }
void ExpandTemplatesVisitor::visit(NumberNode *) { }
void ExpandTemplatesVisitor::visit(CallNode *) { }
void ExpandTemplatesVisitor::visit(VarInferTypeDeclarationNode *) { }
void ExpandTemplatesVisitor::visit(ImportNode *) { }
void ExpandTemplatesVisitor::visit(AsmArrayNode *) { }

