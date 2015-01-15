#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>
#include <string>
#include <memory>
#include "astvisitor.hpp"

class Scope;
class AST;

using ASTNode = std::unique_ptr<AST>;
using ASTChildren = std::vector<AST*>;

class AST
{
public:

    virtual ~AST();
    virtual void build_scope();
    virtual ASTChildren getChildren() const;
    virtual ASTNode copyTree() const = 0;
    virtual std::string toString() const = 0;
    virtual void accept(ASTVisitor& visitor) = 0;

    std::shared_ptr<Scope> scope;
};

#endif
