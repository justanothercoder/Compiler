#include "functionaltype.hpp"
#include "templateinfo.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"

size_t FunctionalType::sizeOf() const { return 0; }
TypeKind FunctionalType::getTypeKind() const { return TypeKind::OVERLOADEDFUNCTION; }

bool FunctionalType::isConvertableTo(const Type *) const { return false; }
boost::optional<int> FunctionalType::rankOfConversion(const Type *) const { return boost::none; }
const FunctionSymbol* FunctionalType::getConversionTo(const Type *) const { return nullptr; }

boost::optional< std::map<std::string, TemplateParam> > makeMappingOfParams(const TemplateSymbol* tmpl
                                                                          , const std::vector<ParamInfo>& formal_params
                                                                          , FunctionTypeInfo arguments)
{
    std::map<std::string, TemplateParam> template_params_map;

    auto args = arguments.params();

    if ( args.size() != formal_params.size() )
        return boost::none;

    auto it = std::begin(args);

    for ( const auto& param : formal_params )
    {
        if ( TemplateInfo(tmpl, { }).isIn(param.second.name()) )
        {
            if ( template_params_map.count(param.second.name()) )
            {
                if ( boost::get<TypeInfo>(template_params_map[param.second.name()]) != makeTypeInfo(*it) )
                    return boost::none;
            }
            else
            {
                template_params_map[param.second.name()] = makeTypeInfo(*it);
            }
        }

        ++it;
    }

    return std::move(template_params_map);
}
    
std::vector<ConversionInfo> getConversions(std::vector<ValueInfo> arguments, std::vector<VariableType> params)
{
    std::vector<ConversionInfo> conversions;

    for ( size_t i = 0; i < params.size(); ++i )
    {
        auto actual_type = arguments[i].type();
        auto desired_type = params[i];

        conversions.push_back(getConversionInfo(actual_type.base(), desired_type.base()));

        if ( !desired_type.isReference() && desired_type.unqualified() -> getTypeKind() == TypeKind::STRUCT )
            static_cast<const StructSymbol*>(desired_type.unqualified()) -> getCopyConstructor() -> is_used = true;
    }
    
    return conversions;
}

ConversionInfo getConversionInfo(const Type *lhs, const Type *rhs)
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

bool checkValues(std::vector<ValueInfo> arguments, std::vector<VariableType> params)
{
    assert(arguments.size() == params.size());

    for ( size_t i = 0; i < arguments.size(); ++i )
    {
        if ( params[i].isReference() && !arguments[i].isLeftValue() && !arguments[i].type().isReference() && !params[i].isConst() )
            return false;
    }

    return true;
}
    
