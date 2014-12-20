#include "markreturnasinlinevisitor.hpp"
#include "returnnode.hpp"

void MarkReturnAsInlineVisitor::visit(ReturnNode* node)
{
    node -> is_in_inline_call = true;
}

void MarkReturnAsInlineVisitor::visit(IfNode* ) { }
void MarkReturnAsInlineVisitor::visit(DotNode* ) { }
void MarkReturnAsInlineVisitor::visit(ForNode* ) { }
void MarkReturnAsInlineVisitor::visit(AddrNode* ) { }
void MarkReturnAsInlineVisitor::visit(NullNode* ) { }
void MarkReturnAsInlineVisitor::visit(CallNode* ) { }
void MarkReturnAsInlineVisitor::visit(TypeNode* ) { }
void MarkReturnAsInlineVisitor::visit(BreakNode* ) { }
void MarkReturnAsInlineVisitor::visit(WhileNode* ) { }
void MarkReturnAsInlineVisitor::visit(UnaryNode* ) { }
void MarkReturnAsInlineVisitor::visit(ImportNode* ) { }
void MarkReturnAsInlineVisitor::visit(StringNode* ) { }
void MarkReturnAsInlineVisitor::visit(NumberNode* ) { }
void MarkReturnAsInlineVisitor::visit(ExternNode* ) { }
void MarkReturnAsInlineVisitor::visit(ModuleNode* ) { }
void MarkReturnAsInlineVisitor::visit(BracketNode* ) { }
void MarkReturnAsInlineVisitor::visit(VariableNode* ) { }
void MarkReturnAsInlineVisitor::visit(FunctionNode* ) { }
void MarkReturnAsInlineVisitor::visit(StatementNode* ) { }
void MarkReturnAsInlineVisitor::visit(UnsafeBlockNode* ) { }
void MarkReturnAsInlineVisitor::visit(NewExpressionNode* ) { }
void MarkReturnAsInlineVisitor::visit(BinaryOperatorNode* ) { }
void MarkReturnAsInlineVisitor::visit(StructDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(ModuleMemberAccessNode* ) { }
void MarkReturnAsInlineVisitor::visit(FunctionDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(VariableDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(VarInferTypeDeclarationNode* ) { }
void MarkReturnAsInlineVisitor::visit(TemplateStructDeclarationNode* ) { }
