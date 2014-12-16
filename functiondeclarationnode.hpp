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
                            , AST *statements
                            , FunctionTraits traits
                            , bool is_unsafe = false);

    AST* copyTree() const override;

    void build_scope() override;

    Symbol* getDefinedSymbol() const override;

    std::vector<AST*> getChildren() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    std::string name;
    FunctionDeclarationInfo info;
    AST *statements;

    FunctionTraits traits;

    FunctionSymbol *definedSymbol;
    FunctionScope *func_scope;

    std::vector<VariableSymbol*> params_symbols;

    bool is_unsafe;
};

#endif
