#ifndef _FUNCTIONDECLARATIONNODE_HPP_
#define _FUNCTIONDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "functionscope.hpp"
#include "functiontraits.hpp"
#include "typeinfo.hpp"
#include "functiondeclarationinfo.hpp"

class FunctionSymbol;
class VariableSymbol;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(std::string name
                            , FunctionDeclarationInfo info
                            , ASTNode statements
                            , FunctionTraits traits
                            , bool is_unsafe = false);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    void build_scope() override;
    Symbol* getDefinedSymbol() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    AST* body();
    const std::vector<VarSymbol*>& paramsSymbols() const;

    FunctionScope* functionScope() const;
    const FunctionTraits& traits() const;

    const std::string& name() const;

    FunctionDeclarationInfo& info();
    const FunctionDeclarationInfo& info() const;

    void addParamSymbol(std::unique_ptr<VarSymbol> var);
    void setDefinedSymbol(FunctionalSymbol* symbol);

    bool isUnsafe() const;

private:

    std::string name_;
    FunctionDeclarationInfo info_;
    ASTNode statements;

    FunctionTraits traits_;
    
    FunctionalSymbol* defined_symbol;
    std::shared_ptr<FunctionScope> func_scope;

    std::vector<VarSymbol*> params_symbols;

    bool is_unsafe;
};

#endif
