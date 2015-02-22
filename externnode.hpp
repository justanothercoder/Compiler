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
class FunctionalSymbol;

class ExternNode : public DeclarationNode
{
public:

    ExternNode(const std::string& name, FunctionDeclarationInfo info, bool is_unsafe);

    ASTNode copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    Symbol* getDefinedSymbol() const;
    void setDefinedSymbol(FunctionalSymbol* symbol);

    const std::string& name() const;
    const FunctionDeclarationInfo& info() const;
    bool isUnsafe() const;

private:

    std::string name_;
    FunctionDeclarationInfo info_;
    bool is_unsafe;

    FunctionalSymbol* defined_symbol = nullptr;
};

#endif
