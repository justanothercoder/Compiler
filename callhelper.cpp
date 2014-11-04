#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"
#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "noviableoverloaderror.hpp"

CallInfo CallHelper::callCheck(std::string name, const Scope *scope, std::vector<ExprNode*> params)
{
//    for ( auto i : params )
//		i -> check();

    auto params_types = extractTypes(params);

    auto function_sym = resolveOverload(name, scope, params_types);

    if ( function_sym == nullptr )
        throw NoViableOverloadError(name, params_types);

    auto function_info = function_sym -> function_type_info;

    int is_meth = (function_sym -> isMethod() ? 1 : 0);

    for ( int i = function_info.params_types.size() - 1; i >= is_meth; --i )
    {
        auto t = function_info.params_types.at(i);
        if ( t -> isReference() 
         && (!params.at(i - is_meth) -> isLeftValue() 
         && !params.at(i - is_meth) -> getType() -> isReference()) 
         && !t -> isConst() )
            throw SemanticError("parameter is not an lvalue.");
    }

    return getCallInfo(function_sym, params);
}

CallInfo CallHelper::getCallInfo(const FunctionSymbol *function_sym, std::vector<ExprNode*> params)
{
    auto function_info = function_sym -> function_type_info;

    auto params_types = CallHelper::extractTypes(params);

    std::vector<ConversionInfo> conversions;
    std::vector<const FunctionSymbol*> copy_constructors;

    int is_meth = (function_sym -> isMethod() ? 1 : 0);

    for ( size_t i = is_meth; i < function_info.params_types.size(); ++i )
    {
        auto actual_type = params_types.at(i - is_meth);
        auto desired_type = function_info.params_types.at(i);

        bool is_left_value = params.at(i - is_meth) -> isLeftValue();

        conversions.push_back(CallHelper::getConversionInfo(actual_type, desired_type, is_left_value));

        auto copy_constr = (desired_type -> isReference() || desired_type -> getTypeKind() == TypeKind::POINTER) ? nullptr : static_cast<const StructSymbol*>(desired_type -> getSymbol()) -> getCopyConstructor();
        copy_constructors.push_back(copy_constr);
    }

    return CallInfo(function_sym, params_types, conversions, copy_constructors);
}

const OverloadedFunctionSymbol* CallHelper::getOverloadedFunc(std::string name, const Scope *scope)
{
    auto _ = scope -> resolve(name);

    if ( _ == nullptr || _ -> getSymbolType() != SymbolType::VARIABLE )
        throw SemanticError("No such symbol " + name + ".");

    return dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(_) -> getType() -> getSymbol());
}

const OverloadedFunctionSymbol* CallHelper::getOverloadedMethod(std::string name, const StructSymbol *scope)
{
    auto _ = scope -> resolveMember(name);

    if ( _ == nullptr || _ -> getSymbolType() != SymbolType::VARIABLE )
        throw SemanticError("No such symbol " + name + ".");

    return dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(_) -> getType() -> getSymbol());
}

const FunctionSymbol* CallHelper::resolveOverload(std::string name, const Scope *scope, std::vector<const Type*> params_types)
{
    while ( scope != nullptr )
    {
        const OverloadedFunctionSymbol *ov_func = nullptr;

        try
        {
            ov_func = CallHelper::getOverloadedFunc(name, scope);
        }
        catch ( SemanticError& e )
        {
            return nullptr;
        }

        auto pt = params_types;

        if ( ov_func -> isMethod() )
        {
//			auto t = TypeFactory::getReference(ov_func -> getBaseType());
//			pt.insert(std::begin(pt), t);
            pt.insert(std::begin(pt), ov_func -> getBaseType());
        }

        auto func_sym = ov_func -> getViableOverload(FunctionTypeInfo(pt));

        if ( func_sym == nullptr )
        {
            try
            {
                while ( scope != nullptr && CallHelper::getOverloadedFunc(name, scope) == ov_func )
                    scope = scope -> getEnclosingScope();
                continue;
            }
            catch ( SemanticError& e )
            {
                return nullptr;
            }
        }

        return func_sym;
    }
    return nullptr;
}

std::vector<const Type*> CallHelper::extractTypes(std::vector<ExprNode*> params)
{
    std::vector<const Type*> params_types(params.size());
    std::transform(std::begin(params), std::end(params), std::begin(params_types), [](ExprNode *t)
    {
        return t -> getType();
    });

    return params_types;
}

ConversionInfo CallHelper::getConversionInfo(const Type *lhs, const Type *rhs, bool is_lhs_left_value)
{
    auto _lhs = lhs -> getUnqualifiedType();
    auto _rhs = rhs -> getUnqualifiedType();

//	auto conv = (_lhs == _rhs) ? nullptr : lhs -> getConversionTo(rhs);
    auto conv = (_lhs == _rhs) ? nullptr : _lhs -> getConversionTo(_rhs);

    if ( _lhs != _rhs && conv == nullptr )
        throw SemanticError("Invalid initialization of '" + rhs -> getName() + "' with type '" + lhs -> getName() + "'.");

    ConversionInfo conv_info(conv, lhs -> isReference() && !is_lhs_left_value, !lhs -> isReference());

    conv_info.actual_type  = lhs;
    conv_info.desired_type = rhs;

    return conv_info;
}
