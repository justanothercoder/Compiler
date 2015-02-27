#include "templatefunctionsymbol.hpp"
#include "functionaltype.hpp"
#include "templateinfo.hpp"
#include "variablenode.hpp"
#include "structdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"

TemplateFunctionSymbol::TemplateFunctionSymbol(const std::string& name, TemplateParamsInfo template_params, TemplateDeclarationNode* _holder) 
    : name(name)
    , template_params(template_params)
    , _holder(_holder)
{

}

std::string TemplateFunctionSymbol::getName() const { return name; }

TemplateParamsInfo TemplateFunctionSymbol::templateParams() const { return template_params; }
TemplateDeclarationNode* TemplateFunctionSymbol::holder() const { return _holder; }
    
FunctionSymbol* TemplateFunctionSymbol::overloadOfTemplateFunction(FunctionTypeInfo info, const TemplateArguments& partial) const
{
    auto decl = static_cast<TemplateFunctionDeclarationNode*>(holder());
    auto tmpl = static_cast<const TemplateFunctionSymbol*>(decl -> getDefinedSymbol());
    const auto& function_info = decl -> info();

    if ( auto mapping = makeMappingOfParams(tmpl, function_info.formalParams(), info.params()) )
    {
        auto template_params_map = *mapping;
        auto template_arguments = TemplateArguments(std::begin(partial), std::end(partial));

        for ( auto template_param : tmpl -> templateParams() )
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

        return static_cast<FunctionSymbol*>(new_decl -> getDefinedSymbol());
    }

    return nullptr;
}
    
Symbol* TemplateFunctionSymbol::specializeWith(const TemplateArguments& arguments) 
{
    return nullptr;
}
    
bool TemplateFunctionSymbol::canBeSpecializedWith(const TemplateArguments& args, const FunctionTypeInfo& info)
{
    return overloadOfTemplateFunction(info, args) != nullptr;
}
