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

#include "structsymbol.hpp"
#include "noviableoverloaderror.hpp"
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
    return std::begin(type_info.overloads) -> paramAt(0);
}

SymbolType OverloadedFunctionSymbol::getSymbolType() const { return SymbolType::OVERLOADED_FUNCTION; }

const FunctionSymbol* OverloadedFunctionSymbol::getViableOverload(FunctionTypeInfo params_type) const
{
    auto overloads = getTypeInfo().getPossibleOverloads(params_type);

    auto func_better = [&params_type](const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
    {
        return lhs.rankOfConversion(params_type) < rhs.rankOfConversion(params_type);
    };

    std::vector<FunctionTypeInfo> v(std::begin(overloads), std::end(overloads));
    
    std::sort(std::begin(v), std::end(v), func_better);

    if ( template_function && !params_type.params().empty() )
    {
        auto decl = static_cast<TemplateFunctionDeclarationNode*>(template_function -> holder());
        auto tmpl = static_cast<TemplateFunctionSymbol*>(decl -> getDefinedSymbol());
        const auto& function_info = decl -> info;

        std::map<std::string, TemplateParam> template_params_map;

        auto it = std::begin(params_type.params());

        bool substitution_failure = false;

        for ( const auto& param : function_info.formalParams() )
        {
            if ( TemplateInfo(tmpl, { }).isIn(param.second.type_name) )
            {
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

            auto new_decl = decl -> instantiateWithTemplateInfo(TemplateInfo(tmpl, template_params));
            tmpl -> holder() -> addInstance(template_params, new_decl);

            ExpandTemplatesVisitor expand;
            DefineVisitor define;
            CheckVisitor check;

            for ( auto visitor : std::vector<ASTVisitor*>{&expand, &define, &check} )
                new_decl -> accept(*visitor);

            auto function = static_cast<const FunctionSymbol*>(new_decl -> getDefinedSymbol());
            auto function_info = function -> type().typeInfo();

            if ( v.empty() || func_better(function_info, v.front()) )
                return function;
        }
    }

    return v.empty() ? nullptr : getTypeInfo().symbols.at(v.front());
}
    
void OverloadedFunctionSymbol::setTemplateFunction(TemplateFunctionSymbol* function) const { template_function = function; }    
TemplateFunctionSymbol* OverloadedFunctionSymbol::templateFunction() const { return template_function; }
    
CallInfo OverloadedFunctionSymbol::resolveCall(std::vector<ValueInfo> arguments) const 
{
    if ( isMethod() )
        arguments.insert(std::begin(arguments), ValueInfo(getBaseType(), true));

    std::vector<VariableType> types;
    for ( auto arg : arguments )
        types.push_back(arg.type());

    auto function = getViableOverload(types);

    if ( function == nullptr )
        throw NoViableOverloadError("", types);

    if ( !checkValues(arguments, function -> type().typeInfo().params()) )
        throw SemanticError("lvalue error");

    function -> is_used = true;
    return CallInfo(function, getConversions(arguments, function -> type().typeInfo().params()));
}

std::vector<ConversionInfo> OverloadedFunctionSymbol::getConversions(std::vector<ValueInfo> arguments, std::vector<VariableType> params) const
{
    std::vector<ConversionInfo> conversions;

    for ( size_t i = 0; i < params.size(); ++i )
    {
        auto actual_type = arguments[i].type();
        auto desired_type = params[i];

        Logger::log("Adding conversion from '" + actual_type.getName() + "' to '" + desired_type.getName() + "'");

        conversions.push_back(getConversionInfo(actual_type.base(), desired_type.base()));

        if ( !desired_type.isReference() && desired_type.unqualified() -> getTypeKind() == TypeKind::STRUCT )
            static_cast<const StructSymbol*>(desired_type.unqualified()) -> getCopyConstructor() -> is_used = true;
    }
    
    return conversions;
}

ConversionInfo OverloadedFunctionSymbol::getConversionInfo(const Type *lhs, const Type *rhs) const
{
    if ( !lhs -> isConvertableTo(rhs) )
        throw SemanticError("Invalid initialization of '" + rhs -> getName() + "' with type '" + lhs -> getName() + "'.");

    auto _lhs = lhs -> removeRef();
    auto _rhs = rhs -> removeRef();

    auto conv = (_lhs == _rhs) ? nullptr : _lhs -> getConversionTo(_rhs);

    if ( conv != nullptr )
        conv -> is_used = true;

    return ConversionInfo(conv, rhs);
}

bool OverloadedFunctionSymbol::checkValues(std::vector<ValueInfo> arguments, std::vector<VariableType> params) const
{
    assert(arguments.size() == params.size());

    for ( size_t i = 0; i < arguments.size(); ++i )
    {
        Logger::log("Param type: " + params[i].getName());
        Logger::log("Arg type  : " + arguments[i].type().getName());
        if ( params[i].isReference() && !arguments[i].isLeftValue() && !arguments[i].type().isReference() && !params[i].isConst() )
            return false;
    }

    return true;
}
