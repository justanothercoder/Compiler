#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"
#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "noviableoverloaderror.hpp"

CallInfo CallHelper::callCheck(std::string name, const Scope *scope, std::vector<ExprNode*> params)
{
    auto params_types = extractTypes(params);

    auto function_sym = resolveOverload(name, scope, params_types);

    if ( function_sym == nullptr )
        throw NoViableOverloadError(name, params_types);

    auto function_info = function_sym -> getType() -> getTypeInfo();

    int is_meth = (function_sym -> isMethod() ? 1 : 0);

    for ( int i = function_info.params_types.size() - 1; i >= is_meth; --i )
    {
        auto t = function_info.params_types.at(i);
        if ( t.isReference() 
         && (!params.at(i - is_meth) -> isLeftValue() 
         && !params.at(i - is_meth) -> getType().isReference()) 
         && !t.isConst() )
            throw SemanticError("parameter is not an lvalue.");
    }

    return getCallInfo(function_sym, params);
}

CallInfo CallHelper::getCallInfo(const FunctionSymbol *function_sym, std::vector<ExprNode*> params)
{
    assert(function_sym != nullptr);
    function_sym -> is_used = true;

    auto function_info = function_sym -> getType() -> getTypeInfo();

    auto params_types = CallHelper::extractTypes(params);

    std::vector<ConversionInfo> conversions;

    int is_meth = (function_sym -> isMethod() ? 1 : 0);

    for ( size_t i = is_meth; i < function_info.params_types.size(); ++i )
    {
        auto actual_type = params_types.at(i - is_meth);
        auto desired_type = function_info.params_types.at(i);

        conversions.push_back(CallHelper::getConversionInfo(actual_type.base(), desired_type.base()));

        if ( !desired_type.isReference() && desired_type.unqualified() -> getTypeKind() == TypeKind::STRUCT )
            static_cast<const StructSymbol*>(desired_type.unqualified()) -> getCopyConstructor() -> is_used = true;
    }

    return CallInfo(function_sym, conversions);
}

const OverloadedFunctionSymbol* CallHelper::getOverloadedFunc(std::string name, const Scope *scope)
{
    auto sym = scope -> resolve(name);

    assert(sym != nullptr);
    if ( sym -> getSymbolType() != SymbolType::OVERLOADED_FUNCTION )
        return nullptr;

    return static_cast<const OverloadedFunctionSymbol*>(sym);
}

const FunctionSymbol* CallHelper::resolveOverload(std::string name, const Scope *scope, std::vector<VariableType> params_types)
{
    while ( scope != nullptr )
    {
        const OverloadedFunctionSymbol *ov_func = getOverloadedFunc(name, scope);

        if ( ov_func == nullptr )
            return nullptr;

        auto pt = params_types;

        if ( ov_func -> isMethod() )
            pt.insert(std::begin(pt), ov_func -> getBaseType());

        auto func_sym = ov_func -> getViableOverload(FunctionTypeInfo(pt));

        if ( func_sym == nullptr )
        {
            while ( scope != nullptr && getOverloadedFunc(name, scope) == ov_func )
                scope = scope -> getEnclosingScope();
            continue;
        }

        return func_sym;
    }
    return nullptr;
}

std::vector<VariableType> CallHelper::extractTypes(std::vector<ExprNode*> params)
{
    std::vector<VariableType> params_types;
    for ( const auto& param : params )
        params_types.push_back(param -> getType());

    return params_types;
}

ConversionInfo CallHelper::getConversionInfo(const Type *lhs, const Type *rhs)
{
    if ( !lhs -> isConvertableTo(rhs) )
        throw SemanticError("Invalid initialization of '" + rhs -> getName() + "' with type '" + lhs -> getName() + "'.");

    auto _lhs = lhs -> getUnqualifiedType();
    auto _rhs = rhs -> getUnqualifiedType();

    auto conv = (_lhs == _rhs) ? nullptr : _lhs -> getConversionTo(_rhs);

    ConversionInfo conv_info(conv);

    conv_info.desired_type = rhs;
    
    if ( conv != nullptr )
        conv -> is_used = true;

    return conv_info;
}
