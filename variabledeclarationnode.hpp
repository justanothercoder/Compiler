#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "typeinfo.hpp"
#include "callinfo.hpp"
#include "inlineinfo.hpp"

class VariableSymbol;

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(const std::string& name, TypeInfo type_info, bool is_field = false, std::vector<ASTExprNode> constructor_params = {});

    void build_scope() override;
    void accept(ASTVisitor& visitor) override;

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    const Symbol* getDefinedSymbol() const override;
    void setDefinedSymbol(std::shared_ptr<const VariableSymbol> sym);

    std::string toString() const override;

    const std::string& name() const;
    bool isField() const;
    const std::vector<ASTExprNode>& constructorParams() const;

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const TypeInfo& typeInfo() const;
    void typeInfo(const TypeInfo& type_info);
    
    const InlineInfo& inlineInfo() const;        
    void inlineInfo(InlineInfo inline_info);

private:
    std::string name_;
    TypeInfo type_info;

    bool is_field;

    std::shared_ptr<const VariableSymbol> defined_symbol;
    std::vector<ASTExprNode> constructor_params;

    CallInfo call_info;
    InlineInfo inline_info;    
};

#endif
