#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "typeinfo.hpp"
#include "nodewithcall.hpp"

class VariableSymbol;

class VariableDeclarationNode : public DeclarationNode, public NodeWithCall
{
public:

    VariableDeclarationNode(const std::string& name, TypeInfo type_info, bool is_field = false, std::vector<ASTExprNode> constructor_params = {});

    void build_scope() override;
    void accept(ASTVisitor& visitor) override;

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    Symbol* getDefinedSymbol() const override;
    void setDefinedSymbol(VarSymbol* sym);

    std::string toString() const override;

    const TypeInfo& typeInfo() const;
    void typeInfo(const TypeInfo& type_info);

    const std::string& name() const;
    bool isField() const;
    const std::vector<ASTExprNode>& constructorParams() const;

private:
    std::string name_;
    TypeInfo type_info;

    bool is_field;

    VarSymbol* defined_symbol;
    std::vector<ASTExprNode> constructor_params;
};

#endif
