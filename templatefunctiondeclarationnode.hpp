#ifndef _TEMPLATEFUNCTIONDECLARATIONNODE_HPP_
#define _TEMPLATEFUNCTIONDECLARATIONNODE_HPP_

#include <map>
#include "templatedeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templateparam.hpp"
#include "functiondeclarationinfo.hpp"
#include "functiontraits.hpp"

class TemplateFunctionSymbol;

class TemplateFunctionDeclarationNode : public TemplateDeclarationNode
{
public:

    TemplateFunctionDeclarationNode(const std::string& name
                                  , FunctionDeclarationInfo info
                                  , ASTNode statements
                                  , FunctionTraits traits
                                  , bool is_unsafe
                                  , TemplateParamsList template_params);

    void build_scope() override;
    void accept(ASTVisitor& visitor);
    
    ASTNode copyTree() const override;
    std::string toString() const override;
    const Symbol* getDefinedSymbol() const override;
    
    unsigned long long hashTemplateParams(std::vector<TemplateParam> template_params) const;
    
    void addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> decl) override;
    std::shared_ptr<DeclarationNode> getInstance(std::vector<TemplateParam> template_params) const override;
    std::shared_ptr<DeclarationNode> instantiateWithTemplateInfo(TemplateInfo info) override;
    std::vector< std::shared_ptr<DeclarationNode> > allInstances() const override;
    
public:

    const FunctionDeclarationInfo& info() const;

private:

    std::string name;
    FunctionDeclarationInfo info_;
    ASTNode statements;
    FunctionTraits traits;
    bool is_unsafe;

    std::map<long long, std::shared_ptr<DeclarationNode> > instances;
    TemplateParamsList template_params;

    std::shared_ptr<TemplateFunctionSymbol> defined_symbol;
};

#endif
