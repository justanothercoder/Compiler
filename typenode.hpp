#ifndef _TYPENODE_HPP_
#define _TYPENODE_HPP_

#include "exprnode.hpp"

class Symbol;
class BuiltInTypeSymbol;
class StructSymbol;

class TypeNode : public ExprNode
{

    friend class TemplateStructSymbol;
    friend class Scope;

    friend class GenSSAVisitor;
    friend class CheckVisitor;
    friend class IsDefinedVisitor;

public:

    TypeNode(std::string name);

    AST* copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    std::string name;

    Symbol *type_symbol;
};

#endif
