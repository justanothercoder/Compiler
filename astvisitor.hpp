#ifndef _ASTVISITOR_HPP_
#define _ASTVISITOR_HPP_

class IfNode;
class ForNode;
class WhileNode;
class BracketNode;
class UnaryNode;
class AddrNode;
class TypeNode;
class NullNode;
class DotNode;
class CallNode;
class BreakNode;
class ReturnNode;
class ImportNode;
class StringNode;
class NumberNode;
class ExternNode;
class ModuleNode;
class VariableNode;
class FunctionNode;
class StatementNode;
class UnsafeBlockNode;
class NewExpressionNode;
class BinaryOperatorNode;
class StructDeclarationNode;
class ModuleMemberAccessNode;
class FunctionDeclarationNode;
class VariableDeclarationNode;
class VarInferTypeDeclarationNode;
class TemplateStructDeclarationNode;
class TemplateFunctionDeclarationNode;
class TemplateFunctionNode;

class ASTVisitor
{
public:
    virtual void visit(IfNode* node) = 0;
    virtual void visit(ForNode* node) = 0;
    virtual void visit(DotNode* node) = 0;
    virtual void visit(AddrNode* node) = 0;
    virtual void visit(TypeNode* node) = 0;
    virtual void visit(CallNode* node) = 0;
    virtual void visit(NullNode* node) = 0;
    virtual void visit(BreakNode* node) = 0;
    virtual void visit(WhileNode* node) = 0;
    virtual void visit(UnaryNode* node) = 0;
    virtual void visit(StringNode* node) = 0;
    virtual void visit(ReturnNode* node) = 0;
    virtual void visit(NumberNode* node) = 0;
    virtual void visit(ExternNode* node) = 0;
    virtual void visit(ModuleNode* node) = 0;
    virtual void visit(BracketNode* node) = 0;
    virtual void visit(ImportNode* node) = 0;
    virtual void visit(FunctionNode* node) = 0;
    virtual void visit(VariableNode* node) = 0;
    virtual void visit(StatementNode* node) = 0;
    virtual void visit(UnsafeBlockNode* node) = 0;
    virtual void visit(NewExpressionNode* node) = 0;
    virtual void visit(BinaryOperatorNode* node) = 0;
    virtual void visit(ModuleMemberAccessNode* node) = 0;
    virtual void visit(StructDeclarationNode* node) = 0;
    virtual void visit(FunctionDeclarationNode* node) = 0;
    virtual void visit(VariableDeclarationNode* node) = 0;
    virtual void visit(VarInferTypeDeclarationNode* node) = 0;
    virtual void visit(TemplateStructDeclarationNode* node) = 0;
    virtual void visit(TemplateFunctionDeclarationNode* node) = 0;
    virtual void visit(TemplateFunctionNode* node) = 0;
};

#endif
