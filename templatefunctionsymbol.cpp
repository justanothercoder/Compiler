#include "templatefunctionsymbol.hpp"
#include "functionaltype.hpp"
#include "templateinfo.hpp"
#include "variablenode.hpp"
#include "structdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "templatespecializationerror.hpp"
#include "logger.hpp"

TemplateFunctionSymbol::TemplateFunctionSymbol(std::string name, TemplateParamsInfo template_params, TemplateDeclarationNode* _holder) 
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
        
        {
            const auto& template_params = tmpl -> templateParams();
            auto it = std::begin(template_params);
            for ( size_t i = 0; i < partial.size() && it != std::end(template_params); ++i)
                ++it;
            for ( ; it != std::end(template_params); ++it )
            {
                const auto& template_param = *it;

                Logger::log("Searching for " + template_param.name());

                if ( template_params_map.count(template_param.name()) )
                    template_arguments.push_back(template_params_map[template_param.name()]);
            }
        }

        if ( template_params.size() != template_arguments.size() )
            throw TemplateSpecializationError(this, template_arguments);

        auto new_decl = decl -> instantiateWithArguments(template_arguments);
        tmpl -> holder() -> addInstance(template_arguments, new_decl);

        ExpandTemplatesVisitor expand;
        DefineVisitor define;
        CheckVisitor check;

        try
        {   
            for ( auto visitor : std::vector<ASTVisitor*>{&expand, &define, &check} )
                new_decl -> accept(*visitor);
        }
        catch ( SemanticError& e ) 
        {
            throw TemplateSpecializationError(this, template_arguments);
        }

        return static_cast<FunctionSymbol*>(new_decl -> getDefinedSymbol());
    }

    return nullptr;
}
    
Symbol* TemplateFunctionSymbol::specializeWith(const TemplateArguments& ) 
{
    return nullptr;
}
    
bool TemplateFunctionSymbol::canBeSpecializedWith(const TemplateArguments& args, const FunctionTypeInfo& info)
{
    return overloadOfTemplateFunction(info, args) != nullptr;
}
