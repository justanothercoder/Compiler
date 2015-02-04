#include "templatefunctionsymbol.hpp"
#include "functionaltype.hpp"
#include "templateinfo.hpp"
#include "templatefunctionsymboldefine.hpp"
#include "variablenode.hpp"
#include "structdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"

TemplateFunctionSymbol::TemplateFunctionSymbol(const std::string& name, TemplateParamsInfo template_symbols, TemplateDeclarationNode* _holder) 
    : name(name)
    , template_symbols(template_symbols)
    , _holder(_holder)
{

}

std::string TemplateFunctionSymbol::getName() const { return name; }

TemplateParamsInfo TemplateFunctionSymbol::templateSymbols() const { return template_symbols; }
TemplateDeclarationNode* TemplateFunctionSymbol::holder() const { return _holder; }
    
std::unique_ptr<DefineSymbolVisitor> TemplateFunctionSymbol::defineSymbolVisitor() const { return std::make_unique<TemplateFunctionSymbolDefine>(); }

const FunctionSymbol* TemplateFunctionSymbol::overloadOfTemplateFunction(FunctionTypeInfo info, const TemplateArguments& partial) const
{
    auto decl = static_cast<TemplateFunctionDeclarationNode*>(holder());
    auto tmpl = static_cast<const TemplateFunctionSymbol*>(decl -> getDefinedSymbol());
    const auto& function_info = decl -> info();

    if ( auto mapping = makeMappingOfParams(tmpl, function_info.formalParams(), info.params()) )
    {
        auto template_params_map = *mapping;
        auto template_arguments = TemplateArguments(std::begin(partial), std::end(partial));

        for ( auto template_param : tmpl -> templateSymbols() )
        {
            if ( template_params_map.count(template_param.first) )
                template_arguments.push_back(template_params_map[template_param.first]);
        }

        auto new_decl = decl -> instantiateWithArguments(template_arguments);
        tmpl -> holder() -> addInstance(template_arguments, new_decl);

        ExpandTemplatesVisitor expand;
        DefineVisitor define;
        CheckVisitor check;

        for ( auto visitor : std::vector<ASTVisitor*>{&expand, &define, &check} )
            new_decl -> accept(*visitor);

        return static_cast<const FunctionSymbol*>(new_decl -> getDefinedSymbol());
    }

    return nullptr;
}

