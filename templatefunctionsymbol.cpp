#include "templatefunctionsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"
#include "templatefunctionsymboldefine.hpp"
#include "variablenode.hpp"

TemplateFunctionSymbol::TemplateFunctionSymbol(std::string name, TemplateParamsList template_symbols, TemplateDeclarationNode* _holder) 
    : name(name)
    , template_symbols(template_symbols)
    , _holder(_holder)
{

}

std::string TemplateFunctionSymbol::getName() const { return name; }
SymbolType TemplateFunctionSymbol::getSymbolType() const { return SymbolType::TEMPLATESTRUCT; }

TemplateParamsList TemplateFunctionSymbol::templateSymbols() const { return template_symbols; }
TemplateDeclarationNode* TemplateFunctionSymbol::holder() const { return _holder; }
    
ScopeVisitor& TemplateFunctionSymbol::getScopeVisitor() { return *(new TemplateFunctionSymbolDefine(this)); }
