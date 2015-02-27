#ifndef _VARINFERTYPEDECLARATIONNODE_HPP_
#define _VARINFERTYPEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "nodewithcall.hpp"
#include "exprnode.hpp"

class VariableSymbol;

class VarInferTypeDeclarationNode : public DeclarationNode, public NodeWithCall
{
public:

    VarInferTypeDeclarationNode(std::string name, ASTExprNode expr);

    void build_scope();

    Symbol* getDefinedSymbol() const override;
    void setDefinedSymbol(VarSymbol* symbol);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;
    ExprNode* expr();
    
private:

    std::string name_;
    ASTExprNode expr_;

    VarSymbol* defined_symbol;
};

#endif
