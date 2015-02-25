#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"
#include "templatedeclarationnode.hpp"

TemplateStructSymbol::TemplateStructSymbol(std::string name, TemplateParamsInfo template_params, TemplateDeclarationNode* _holder) 
    : name(name)
    , template_params(template_params)
    , _holder(_holder)
{

}

std::string TemplateStructSymbol::getName() const { return name; }

TemplateParamsInfo TemplateStructSymbol::templateParams() const { return template_params; }
TemplateDeclarationNode* TemplateStructSymbol::holder() const { return _holder; }

Symbol* TemplateStructSymbol::specializeWith(const TemplateArguments& arguments) 
{
    return holder() -> instantiateWithArguments(arguments) -> getDefinedSymbol();
}
