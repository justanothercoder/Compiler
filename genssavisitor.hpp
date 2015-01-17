#ifndef _GENSSAVISITOR_HPP_
#define _GENSSAVISITOR_HPP_

#include "astvisitor.hpp"
#include "threeaddresscode.hpp"
#include "compiler.hpp"

class AST;
class CallableNode;
struct InlineInfo;

class GenSSAVisitor : public ASTVisitor, public Compiler
{

    friend class Optimizer;

public:

    GenSSAVisitor(ThreeAddressCode& code);

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
    void visit(TemplateFunctionDeclarationNode* node) override;
    void visit(TemplateFunctionNode* node) override;
    
private:

    const std::shared_ptr<Arg>& getArg(AST* node);
    void generateCall(std::vector<Argument> args, const CallInfo& call_info, const InlineInfo& inline_info);

    bool isIntType(const Type* t);
    bool isCharType(const Type* t);
    bool isPointer(const Type* t);
    bool isReference(const Type* t);
    bool isSimpleType(const Type* t);

    void genParam(ExprNode* node, ConversionInfo conversion_info);
    void genCall(const FunctionSymbol* func, int params_size);
    void genInlineCall(const FunctionSymbol* function, const InlineInfo& inline_info, std::vector<Argument> params);

    std::stack< std::pair<Argument, Argument> > loop_label;

    Argument _arg = nullptr;
    ThreeAddressCode& code;
};

#endif
