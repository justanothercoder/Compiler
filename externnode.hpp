#ifndef _EXTERNNODE_HPP_
#define _EXTERNNODE_HPP_

#include <string>
#include <vector>
#include <utility>

#include "ast.hpp"
#include "typeinfo.hpp"
#include "declarationnode.hpp"
#include "functiondeclarationinfo.hpp"

class FunctionSymbol;

class ExternNode : public DeclarationNode
{
public:

    ExternNode(const std::string& name, FunctionDeclarationInfo info, bool is_unsafe);

    ASTNode copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const Symbol* getDefinedSymbol() const;
    void setDefinedSymbol(std::shared_ptr<const FunctionSymbol> symbol);

    const std::string& name() const;
    const FunctionDeclarationInfo& info() const;
    bool isUnsafe() const;

private:

    std::string name_;
    FunctionDeclarationInfo info_;
    bool is_unsafe;

    std::shared_ptr<const FunctionSymbol> defined_symbol = nullptr;
};

#endif
