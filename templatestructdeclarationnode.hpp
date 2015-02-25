#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include <map>
#include "templatedeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templateargument.hpp"
#include "templatestructsymbol.hpp"

class TemplateStructDeclarationNode : public TemplateDeclarationNode
{
    friend class ExpandTemplatesVisitor;
    friend class DefineTypesVisitor;

public:

    TemplateStructDeclarationNode(std::string name, std::vector<ASTNode> inner, TemplateParamsInfo template_params);

    void build_scope() override;
    void accept(ASTVisitor& visitor);
    
    ASTNode copyTree() const override;
    std::string toString() const override;
    Symbol* getDefinedSymbol() const override;

    std::string name() const;
    const TemplateParamsInfo& templateParamsInfo() const;
    
    void addInstance(TemplateArguments template_params, std::shared_ptr<DeclarationNode> decl) override;
    std::shared_ptr<DeclarationNode> getInstance(TemplateArguments template_params) const override;
    std::shared_ptr<DeclarationNode> instantiateWithArguments(TemplateArguments params) override;
    std::vector<DeclarationNode*> allInstances() const override;

private:

    std::string name_;
    std::vector<ASTNode> inner;

    std::map<long long, std::shared_ptr<DeclarationNode> > instances;
    TemplateParamsInfo template_params;

    TemplateStructSymbol* defined_symbol;
};

#endif
