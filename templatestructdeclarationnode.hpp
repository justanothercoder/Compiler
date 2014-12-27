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

    TemplateStructDeclarationNode(std::string name, std::vector<AST*> inner, std::vector< std::pair<std::string, TypeInfo> > template_params);

    void build_scope() override;

    void accept(ASTVisitor& visitor);
    
    AST* copyTree() const override;
    std::string toString() const override;
    Symbol* getDefinedSymbol() const override;
    
    void addInstance(std::vector<TemplateParam> template_params, DeclarationNode* decl) override;
    DeclarationNode* getInstance(std::vector<TemplateParam> template_params) const override;
    DeclarationNode* instantiateWithTemplateInfo(TemplateInfo info) override;
    std::vector<DeclarationNode*> allInstances() const override;
    
private:

    std::string name;
    std::vector<AST*> inner;

    std::map<long long, DeclarationNode*> instances;
    TemplateParamsList template_params;

    TemplateStructSymbol* defined_symbol;
};

#endif
