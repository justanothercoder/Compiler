#include "partiallyinstantiatedfunctionsymbol.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "templatefunctionsymbol.hpp"
#include "logger.hpp"

PartiallyInstantiatedFunctionSymbol::PartiallyInstantiatedFunctionSymbol(const OverloadedFunctionSymbol* ov_func
                                                                       , TemplateArguments tmpl_arguments) : ov_func(ov_func)
                                                                                                           , tmpl_arguments(tmpl_arguments)
{

}

CallInfo PartiallyInstantiatedFunctionSymbol::resolveCall(std::vector<ValueInfo> arguments) const 
{
    std::vector<VariableType> types;
    for ( auto arg : arguments )
        types.push_back(arg.type());

    if ( auto overload = ov_func -> template_function -> overloadOfTemplateFunction(types, tmpl_arguments) )
    {
        if ( !checkValues(arguments, overload -> type().typeInfo().params()) )
            throw SemanticError("lvalue error");

        overload -> is_used = true;
        return CallInfo(overload, getConversions(arguments, overload -> type().typeInfo().params()));
    }
    else
        return ov_func -> resolveCall(arguments);
}
    
std::string PartiallyInstantiatedFunctionSymbol::typeName() const 
{
    return ov_func -> getName();
}
