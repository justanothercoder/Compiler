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
                            , bool is_unsafe = false
                            , boost::optional<TemplateInfo> template_info = boost::none);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    void build_scope() override;
    const Symbol* getDefinedSymbol() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    AST* body();
    const std::vector< std::shared_ptr<VariableSymbol> >& paramsSymbols() const;

    FunctionScope* functionScope() const;
    const FunctionTraits& traits() const;

    const std::string& name() const;

    FunctionDeclarationInfo& info();
    const FunctionDeclarationInfo& info() const;

    void addParamSymbol(std::shared_ptr<VariableSymbol> var);
    void setDefinedSymbol(std::shared_ptr<const FunctionSymbol> symbol);

    bool isUnsafe() const;

private:

    boost::optional<TemplateInfo> template_info;

    std::string name_;
    FunctionDeclarationInfo info_;
    ASTNode statements;

    FunctionTraits traits_;
    
    std::shared_ptr<const FunctionSymbol> defined_symbol;
    std::shared_ptr<FunctionScope> func_scope;

    std::vector< std::shared_ptr<VariableSymbol> > params_symbols;

    bool is_unsafe;
};

#endif
