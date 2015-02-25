#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>
#include <boost/optional.hpp>

#include "declarationnode.hpp"
#include "templateinfo.hpp"

class TypeSymbol;
class StructScope;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(const std::string& name, std::vector<ASTNode> inner);
    
    void build_scope();
    Symbol* getDefinedSymbol() const override;

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;
    const std::vector<ASTNode>& inner() const;

    StructScope* structScope() const;

    void setDefinedSymbol(TypeSymbol* type);

private:

    std::string name_;
    std::vector<ASTNode> inner_;

    TypeSymbol* defined_symbol = nullptr;
    std::shared_ptr<StructScope> struct_scope = nullptr;
};

#endif
