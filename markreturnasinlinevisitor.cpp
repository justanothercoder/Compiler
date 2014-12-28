#include "markreturnasinlinevisitor.hpp"
#include "returnnode.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "whilenode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "functionnode.hpp"
#include "callnode.hpp"
#include "scope.hpp"
#include "functionsymbol.hpp"

#include "logger.hpp"

void MarkReturnAsInlineVisitor::visit(ReturnNode* node)
{
    node -> is_in_inline_call = true;
    node -> expr -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(IfNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(ForNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(WhileNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(StatementNode* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(UnsafeBlockNode* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(CallNode* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void MarkReturnAsInlineVisitor::visit(DotNode* ) { }
void MarkReturnAsInlineVisitor::visit(AddrNode* ) { }
void MarkReturnAsInlineVisitor::visit(NullNode* ) { }
void MarkReturnAsInlineVisitor::visit(TypeNode* ) { }
void MarkReturnAsInlineVisitor::visit(BreakNode* ) { }
void MarkReturnAsInlineVisitor::visit(UnaryNode* ) { }
void MarkReturnAsInlineVisitor::visit(ImportNode* ) { }
void MarkReturnAsInlineVisitor::visit(StringNode* ) { }
void MarkReturnAsInlineVisitor::visit(NumberNode* ) { }
void MarkReturnAsInlineVisitor::visit(ExternNode* ) { }
void MarkReturnAsInlineVisitor::visit(ModuleNode* ) { }
void MarkReturnAsInlineVisitor::visit(BracketNode* ) { }
void MarkReturnAsInlineVisitor::visit(VariableNode* ) { }
void MarkReturnAsInlineVisitor::visit(FunctionNode* ) { } 
void MarkReturnAsInlineVisitor::visit(NewExpressionNode* ) { }
void MarkReturnAsInlineVisitor::visit(BinaryOperatorNode* ) { }
void MarkReturnAsInlineVisitor::visit(StructDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(ModuleMemberAccessNode* ) { }
void MarkReturnAsInlineVisitor::visit(FunctionDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(VariableDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(VarInferTypeDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(TemplateStructDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(TemplateFunctionDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(TemplateFunctionNode* ) { }
