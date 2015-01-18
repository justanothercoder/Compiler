#include "overloadedfunctionsymbol.hpp"
#include "functionsymbol.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "templatefunctionsymbol.hpp"
#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "structsymbol.hpp"
#include "noviableoverloaderror.hpp"
#include "logger.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(std::string name, FunctionTraits traits) : name(name), type_info({ }), traits(traits) { }

std::string OverloadedFunctionSymbol::getName() const { return name; }
const OverloadedFunctionTypeInfo& OverloadedFunctionSymbol::getTypeInfo() const { return type_info; }

void OverloadedFunctionSymbol::addOverload(FunctionTypeInfo func_type_info, std::shared_ptr<const Symbol> sym) const
{
    type_info.overloads.insert(func_type_info);
    type_info.symbols.emplace(func_type_info, sym);
}

VariableType OverloadedFunctionSymbol::getBaseType() const
{
    assert(isMethod());
    return std::begin(type_info.overloads) -> paramAt(0);
}

SymbolType OverloadedFunctionSymbol::getSymbolType() const { return SymbolType::OVERLOADED_FUNCTION; }

const FunctionSymbol* OverloadedFunctionSymbol::getViableOverload(FunctionTypeInfo params_type) const
{
    auto overloads = type_info.getPossibleOverloads(params_type);

    auto func_better = [&params_type](const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
    {
        return lhs.rankOfConversion(params_type) < rhs.rankOfConversion(params_type);
    };

    auto v = std::vector<FunctionTypeInfo>(std::begin(overloads), std::end(overloads));
    std::sort(std::begin(v), std::end(v), func_better);

    if ( template_function && !params_type.params().empty() )
    {
        if ( auto overload = overloadOfTemplateFunction(template_function, params_type) )
        {
            if ( v.empty() || func_better(overload -> type().typeInfo(), v.front()) )
                return overload;
        }
    }

    return v.empty() ? nullptr : static_cast<const FunctionSymbol*>(type_info.symbols.at(v.front()).get());
}
    
void OverloadedFunctionSymbol::setTemplateFunction(const TemplateFunctionSymbol* function) const { template_function = function; }    
const TemplateFunctionSymbol* OverloadedFunctionSymbol::templateFunction() const { return template_function; }
    
CallInfo OverloadedFunctionSymbol::resolveCall(std::vector<ValueInfo> arguments) const 
{
    if ( isMethod() )
        arguments.insert(std::begin(arguments), {getBaseType(), true});

    std::vector<VariableType> types;
    for ( auto arg : arguments )
        types.push_back(arg.type());

    auto function = getViableOverload(types);

    if ( function == nullptr )
        throw NoViableOverloadError("", types);

    auto function_params = function -> type().typeInfo().params();

    if ( !checkValues(arguments, function_params) )
        throw SemanticError("lvalue error");

    function -> is_used = true;
    return CallInfo(function, getConversions(arguments, function_params));
}

const FunctionSymbol* OverloadedFunctionSymbol::overloadOfTemplateFunction(const TemplateFunctionSymbol* template_function
                                                                         , FunctionTypeInfo info
                                                                         , const std::vector<TemplateParam>& partial) const
{
    auto decl = static_cast<TemplateFunctionDeclarationNode*>(template_function -> holder());
    auto tmpl = static_cast<const TemplateFunctionSymbol*>(decl -> getDefinedSymbol());
    const auto& function_info = decl -> info();

    if ( auto mapping = makeMappingOfParams(tmpl, function_info.formalParams(), info.params()) )
    {
        auto template_params_map = *mapping;

        auto template_params = std::vector<TemplateParam>(std::begin(partial), std::end(partial));

        for ( auto template_param : tmpl -> templateSymbols() )
        {
            if ( template_params_map.count(template_param.first) )
                template_params.push_back(template_params_map[template_param.first]);
        }

        auto new_decl = decl -> instantiateWithTemplateInfo(TemplateInfo(tmpl, template_params));
        tmpl -> holder() -> addInstance(template_params, new_decl);

        ExpandTemplatesVisitor expand;
        DefineVisitor define;
        CheckVisitor check;

        for ( auto visitor : std::vector<ASTVisitor*>{&expand, &define, &check} )
            new_decl -> accept(*visitor);

        return static_cast<const FunctionSymbol*>(new_decl -> getDefinedSymbol());
    }

    return nullptr;
}

bool OverloadedFunctionSymbol::isMethod() const { return traits.is_method; }
