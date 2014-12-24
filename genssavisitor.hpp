#ifndef _GENSSAVISITOR_HPP_
#define _GENSSAVISITOR_HPP_

#include "astvisitor.hpp"
#include "threeaddresscode.hpp"
#include "compiler.hpp"

class AST;

class GenSSAVisitor : public ASTVisitor, public Compiler
{

    friend class Optimizer;

public:

    GenSSAVisitor(ThreeAddressCode& code);

    Arg* getArg(AST* node);

    void visit(IfNode* node) override;
    void visit(ForNode* node) override;
    void visit(DotNode* node) override;
    void visit(TypeNode* node) override;
    void visit(AddrNode* node) override;
    void visit(NullNode* node) override;
    void visit(CallNode* node) override;
    void visit(WhileNode* node) override;
    void visit(UnaryNode* node) override;
    void visit(BreakNode* node) override;
    void visit(ImportNode* node) override;
    void visit(StringNode* node) override;
    void visit(NumberNode* node) override;
    void visit(ReturnNode* node) override;
    void visit(ModuleNode* node) override;
    void visit(ExternNode* node) override;
    void visit(BracketNode* node) override;
    void visit(FunctionNode* node) override;
    void visit(VariableNode* node) override;
    void visit(StatementNode* node) override;
    void visit(UnsafeBlockNode* node) override;
    void visit(NewExpressionNode* node) override;
    void visit(BinaryOperatorNode* node) override;
    void visit(ModuleMemberAccessNode* node) override;
    void visit(StructDeclarationNode* node) override;
    void visit(FunctionDeclarationNode* node) override;
    void visit(VariableDeclarationNode* node) override;
    void visit(VarInferTypeDeclarationNode* node) override;
    void visit(TemplateStructDeclarationNode* node) override;
    
private:

    bool isIntType(const Type* t);
    bool isCharType(const Type* t);
    bool isPointer(const Type* t);
    bool isReference(const Type* t);
    bool isSimpleType(const Type* t);

    void genParam(ExprNode* node, ConversionInfo conversion_info);
    void genCall(const FunctionSymbol* func, int params_size);
    void genInlineCall(const FunctionSymbol* function, AST* inline_call_body, const std::vector<VariableSymbol*>& inline_locals, std::vector<Arg*> params, Arg* this_expr);

    std::stack< std::pair<Arg*, Arg*> > loop_label;

    Arg* _arg;    
    ThreeAddressCode& code;
};

#endif
