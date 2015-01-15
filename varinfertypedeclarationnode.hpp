#ifndef _VARINFERTYPEDECLARATIONNODE_HPP_
#define _VARINFERTYPEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "callinfo.hpp"
#include "exprnode.hpp"

class VariableSymbol;

class VarInferTypeDeclarationNode : public DeclarationNode
{
public:

    VarInferTypeDeclarationNode(const std::string& name, ASTExprNode expr);

    void build_scope();

    const Symbol* getDefinedSymbol() const override;
    void setDefinedSymbol(std::shared_ptr<const VariableSymbol> symbol);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;
    ExprNode* expr();
    
    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

private:

    std::string name_;
    ASTExprNode expr_;

    CallInfo call_info;
    std::shared_ptr<const VariableSymbol> defined_symbol;
};

#endif
