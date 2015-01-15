#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include <map>
#include "templatedeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templateparam.hpp"
#include "templatestructsymbol.hpp"

class TemplateStructDeclarationNode : public TemplateDeclarationNode
{
public:

    TemplateStructDeclarationNode(std::string name, std::vector<ASTNode> inner, TemplateParamsList template_params);

    void build_scope() override;
    void accept(ASTVisitor& visitor);
    
    ASTNode copyTree() const override;
    std::string toString() const override;
    Symbol* getDefinedSymbol() const override;

    unsigned long long hashTemplateParams(std::vector<TemplateParam> template_params) const;
    
    void addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> decl) override;
    std::shared_ptr<DeclarationNode> getInstance(std::vector<TemplateParam> template_params) const override;
    std::shared_ptr<DeclarationNode> instantiateWithTemplateInfo(TemplateInfo info) override;
    std::vector< std::shared_ptr<DeclarationNode> > allInstances() const override;
    
private:

    std::string name;
    std::vector<ASTNode> inner;

    std::map<long long, std::shared_ptr<DeclarationNode> > instances;
    TemplateParamsList template_params;

    std::shared_ptr<TemplateStructSymbol> defined_symbol;
};

#endif
