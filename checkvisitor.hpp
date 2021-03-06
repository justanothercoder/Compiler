#ifndef _CHECKVISITOR_HPP_
#define _CHECKVISITOR_HPP_

#include <stack>
#include "compiler.hpp"
#include "astvisitor.hpp"
#include "valueinfo.hpp"
#include "callinfo.hpp"
#include "functiontypeinfo.hpp"

class AST;
class FunctionScope;
class FunctionalType;

class CheckVisitor : public ASTVisitor, public Compiler
{
public:

    void visit(IfNode* node) override;
    void visit(DotNode* node) override;
    void visit(ForNode* node) override;
    void visit(AddrNode* node) override;
    void visit(NullNode* node) override;
    void visit(CallNode* node) override;
    void visit(BreakNode* node) override;
    void visit(WhileNode* node) override;
    void visit(UnaryNode* node) override;
    void visit(ImportNode* node) override;
    void visit(StringNode* node) override;
    void visit(NumberNode* node) override;
    void visit(ReturnNode* node) override;
    void visit(ExternNode* node) override;
    void visit(ModuleNode* node) override;
    void visit(LambdaNode* node) override;
    void visit(BracketNode* node) override;
    void visit(VariableNode* node) override;
    void visit(FunctionNode* node) override;
    void visit(StatementNode* node) override;
    void visit(UnsafeBlockNode* node) override;
    void visit(NewExpressionNode* node) override;
    void visit(BinaryOperatorNode* node) override;
    void visit(StructDeclarationNode* node) override;
    void visit(FunctionDeclarationNode* node) override;
    void visit(VariableDeclarationNode* node) override;
    void visit(VarInferTypeDeclarationNode* node) override;
    void visit(TemplateStructDeclarationNode* node) override;
    void visit(TemplateFunctionDeclarationNode* node) override;
    void visit(TemplateFunctionNode* node) override;

private:

    void visitChildren(AST* node);
    CallInfo checkCall(const FunctionalType* function, std::vector<ValueInfo> arguments);

    std::vector<ValueInfo> extractArguments(const std::vector< std::unique_ptr<ExprNode> >& params);

    const FunctionTypeInfo& getCallArguments();
    void pushArguments(FunctionTypeInfo info);
    void popArguments();

private:

    std::stack<FunctionScope*> function_scopes;
    std::stack<FunctionTypeInfo> arguments_stack;
};

#endif
