#ifndef _EXTERNNODE_HPP_
#define _EXTERNNODE_HPP_

#include <string>
#include <vector>
#include <utility>

#include "ast.hpp"
#include "typeinfo.hpp"
#include "functiondeclarationinfo.hpp"

class FunctionSymbol;

class ExternNode : public AST
{
public:

    ExternNode(std::string name, FunctionDeclarationInfo info, bool is_unsafe);

    AST* copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    std::string name;
    FunctionDeclarationInfo info;
    bool is_unsafe;

    FunctionSymbol *definedSymbol;
};

#endif
