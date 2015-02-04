#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"

#include "variablenode.hpp"

TemplateStructSymbol::TemplateStructSymbol(std::string name, TemplateParamsInfo template_symbols, TemplateDeclarationNode* _holder) 
    : name(name)
    , template_symbols(template_symbols)
    , _holder(_holder)
{

}

std::string TemplateStructSymbol::getName() const { return name; }

TemplateParamsInfo TemplateStructSymbol::templateSymbols() const { return template_symbols; }
TemplateDeclarationNode* TemplateStructSymbol::holder() const { return _holder; }
