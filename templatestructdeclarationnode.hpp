#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include <map>
#include "templatedeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templateparam.hpp"
#include "templatestructsymbol.hpp"

class TemplateStructDeclarationNode : public TemplateDeclarationNode
{
    friend class ExpandTemplatesVisitor;

public:

    TemplateStructDeclarationNode(std::string name, std::vector<ASTNode> inner, TemplateParamsList template_params);

    void build_scope() override;
    void accept(ASTVisitor& visitor);
    
    ASTNode copyTree() const override;
    std::string toString() const override;
    Symbol* getDefinedSymbol() const override;

    std::string name() const;
    const TemplateParamsList& templateParams() const;
    
    void addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> decl) override;
    std::shared_ptr<DeclarationNode> getInstance(std::vector<TemplateParam> template_params) const override;
    std::shared_ptr<DeclarationNode> instantiateWithParams(std::vector<TemplateParam> params) override;
    std::vector<DeclarationNode*> allInstances() const override;

private:

    std::string name_;
    std::vector<ASTNode> inner;

    std::map<long long, std::shared_ptr<DeclarationNode> > instances;
    TemplateParamsList template_params;

    std::shared_ptr<TemplateStructSymbol> defined_symbol;
};

#endif
