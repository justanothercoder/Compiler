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

    TemplateFunctionDeclarationNode(std::string name, FunctionDeclarationInfo info, AST* statements, FunctionTraits traits, bool is_unsafe, TemplateParamsList template_params);

    void build_scope() override;

    void accept(ASTVisitor& visitor);
    
    AST* copyTree() const override;
    std::string toString() const override;
    Symbol* getDefinedSymbol() const override;
    
    unsigned long long hashTemplateParams(std::vector<TemplateParam> template_params) const;
    
    void addInstance(std::vector<TemplateParam> template_params, DeclarationNode* decl) override;
    DeclarationNode* getInstance(std::vector<TemplateParam> template_params) const override;
    DeclarationNode* instantiateWithTemplateInfo(TemplateInfo info) override;
    std::vector<DeclarationNode*> allInstances() const override;
    
//private:

    std::string name;
    FunctionDeclarationInfo info;
    AST* statements;
    FunctionTraits traits;
    bool is_unsafe;

    std::map<long long, DeclarationNode*> instances;
    TemplateParamsList template_params;

    TemplateFunctionSymbol* defined_symbol;
};

#endif
