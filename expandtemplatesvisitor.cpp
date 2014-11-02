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

    template_symbols.insert(node -> definedSymbol);
}

void ExpandTemplatesVisitor::visit(VariableDeclarationNode *node) 
{
    const auto& template_info = node -> scope -> getTemplateInfo();
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

