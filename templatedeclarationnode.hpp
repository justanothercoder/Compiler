#ifndef _TEMPLATEDECLARATIONNODE_HPP_
#define _TEMPLATEDECLARATIONNODE_HPP_

#include <memory>
#include "declarationnode.hpp"
#include "templateinfo.hpp"

class TemplateDeclarationNode : public DeclarationNode
{
public:
    virtual void addInstance(TemplateArguments template_arguments, std::shared_ptr<DeclarationNode> decl) = 0;
    virtual std::shared_ptr<DeclarationNode> getInstance(TemplateArguments template_arguments) const = 0;    
    virtual std::shared_ptr<DeclarationNode> instantiateWithArguments(TemplateArguments params) = 0;
    virtual std::vector<DeclarationNode*> allInstances() const = 0;
};

#endif
