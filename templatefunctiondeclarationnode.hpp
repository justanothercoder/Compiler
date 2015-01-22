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
    friend class ExpandTemplatesVisitor;

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
    
    void addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> decl) override;
    std::shared_ptr<DeclarationNode> getInstance(std::vector<TemplateParam> template_params) const override;
    std::shared_ptr<DeclarationNode> instantiateWithParams(std::vector<TemplateParam> params) override;
    std::vector<DeclarationNode*> allInstances() const override;
    
public:

    std::string name() const;
    const FunctionDeclarationInfo& info() const;
    FunctionTraits traits() const;
    bool isUnsafe() const;
    const TemplateParamsList& templateParams() const;

    AST* body();    

private:

    std::string name_;
    FunctionDeclarationInfo info_;
    ASTNode statements;
    FunctionTraits traits_;
    bool is_unsafe;

    std::map<long long, std::shared_ptr<DeclarationNode> > instances;
    TemplateParamsList template_params;

    std::shared_ptr<TemplateFunctionSymbol> defined_symbol;
};

#endif
