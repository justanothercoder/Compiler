#include "overloadedfunctionsymbol.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "functionsymbol.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "pointertype.hpp"
#include "templatefunctionsymbol.hpp"

#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"

#include "logger.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(std::string name
                                                 , OverloadedFunctionTypeInfo type_info
                                                 , FunctionTraits traits) : name     (name)
                                                                          , type_info(type_info)
                                                                          , traits   (traits)
                                                                          , template_function(nullptr)
{

}

std::string OverloadedFunctionSymbol::getName() const { return name; }
size_t OverloadedFunctionSymbol::sizeOf() const { return Comp::config().int_size; }

OverloadedFunctionTypeInfo OverloadedFunctionSymbol::getTypeInfo() const { return type_info; }

void OverloadedFunctionSymbol::addOverload(FunctionTypeInfo func_type_info, FunctionSymbol *sym) const
{
    type_info.overloads.insert(func_type_info);
    type_info.symbols[func_type_info] = sym;
}

bool OverloadedFunctionSymbol::isMethod() const { return traits.is_method; }
bool OverloadedFunctionSymbol::isConstructor() const { return traits.is_constructor; }
bool OverloadedFunctionSymbol::isOperator() const { return traits.is_operator; }

VariableType OverloadedFunctionSymbol::getBaseType() const
{
    if ( !isMethod() )
        throw;
    return std::begin(type_info.overloads) -> params_types[0];
}

SymbolType OverloadedFunctionSymbol::getSymbolType() const { return SymbolType::OVERLOADED_FUNCTION; }
TypeKind OverloadedFunctionSymbol::getTypeKind() const { return TypeKind::OVERLOADEDFUNCTION; }

const FunctionSymbol* OverloadedFunctionSymbol::getViableOverload(FunctionTypeInfo params_type) const
{
    Logger::log("Trying to find an overload for '" + params_type.toString() + "'");

    auto overloads = getTypeInfo().getPossibleOverloads(params_type);

    auto func_better = [&params_type](const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
    {
        return lhs.rankOfConversion(params_type) < rhs.rankOfConversion(params_type);
    };

    std::vector<FunctionTypeInfo> v(std::begin(overloads), std::end(overloads));
    
    std::sort(std::begin(v), std::end(v), func_better);

    if ( template_function && !params_type.params_types.empty() )
    {
        auto decl = static_cast<TemplateFunctionDeclarationNode*>(template_function -> holder());
        auto tmpl = static_cast<TemplateFunctionSymbol*>(decl -> getDefinedSymbol());
        const auto& function_info = decl -> info;

        std::map<std::string, TemplateParam> template_params_map;

        auto it = std::begin(params_type.params_types);

        bool substitution_failure = false;

        for ( const auto& param : function_info.formalParams() )
        {
            Logger::log("Formal param: " + param.second.toString());

            if ( TemplateInfo(tmpl, { }).isIn(param.second.type_name) )
            {
                Logger::log("Actual param: " + it -> getName());

                if ( template_params_map.count(param.second.type_name) )
                {
                    if ( boost::get<TypeInfo>(template_params_map[param.second.type_name]).type_name != it -> unqualified() -> getName() )
                    {
                        substitution_failure = true;
                        break;
                    }
                }
                else
                {
                    int ptr = 0;
                    auto tp = it -> unqualified();
                    while ( tp -> getTypeKind() == TypeKind::POINTER )
                    {
                        tp = static_cast<const PointerType*>(tp) -> pointedType();
                        ++ptr;
                    }

                    auto type_info = TypeInfo(it -> unqualified() -> getName()
                                            , it -> isReference()
                                            , it -> isConst()
                                            , { }
                                            , ptr
                                            , { });

                    Logger::log("Type info: " + type_info.toString());

                    template_params_map[param.second.type_name] = type_info;
                }
            }

            ++it;
        }

        if ( !substitution_failure )
        {
            std::vector<TemplateParam> template_params;

            for ( auto template_param : tmpl -> templateSymbols() )
                template_params.push_back(template_params_map[template_param.first]);

            //
            for ( auto param : template_params )
                Logger::log(boost::get<TypeInfo>(param).toString());
            //            

            auto new_decl = decl -> instantiateWithTemplateInfo(TemplateInfo(tmpl, template_params));

            ExpandTemplatesVisitor expand;
            DefineVisitor define;
            CheckVisitor check;

            for ( auto visitor : std::vector<ASTVisitor*>{&expand, &define, &check} )
                new_decl -> accept(*visitor);

            auto function = static_cast<const FunctionSymbol*>(new_decl -> getDefinedSymbol());

            Logger::log("Function: " + function -> getName());

            auto function_info = function -> type().typeInfo();

            if ( v.empty() || func_better(function_info, v.front()) )
                return function;
        }
    }

    //
        for ( auto ti : v )
            Logger::log(ti.toString());
        Logger::log("");
    //

    return v.empty() ? nullptr : getTypeInfo().symbols.at(v.front());
}

bool OverloadedFunctionSymbol::isConvertableTo(const Type *) const { return false; } 
boost::optional<int> OverloadedFunctionSymbol::rankOfConversion(const Type *) const { return boost::none; } 
FunctionSymbol* OverloadedFunctionSymbol::getConversionTo(const Type *) const { return nullptr; }
    
void OverloadedFunctionSymbol::setTemplateFunction(TemplateFunctionSymbol* function) const { template_function = function; }    
TemplateFunctionSymbol* OverloadedFunctionSymbol::templateFunction() const { return template_function; }
