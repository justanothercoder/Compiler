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

    ExternNode(std::string name, FunctionDeclarationInfo info, bool is_unsafe);

    AST* copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    Symbol* getDefinedSymbol() const;

    std::string name;
    FunctionDeclarationInfo info;
    bool is_unsafe;

    FunctionSymbol* definedSymbol;
};

#endif
