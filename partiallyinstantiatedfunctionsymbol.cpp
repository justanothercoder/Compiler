#include "partiallyinstantiatedfunctionsymbol.hpp"
#include "functionsymbol.hpp"

#include "logger.hpp"

PartiallyInstantiatedFunctionSymbol::PartiallyInstantiatedFunctionSymbol(const OverloadedFunctionSymbol* ov_func
                                                                       , std::vector<TemplateParam> tmpl_arguments) : ov_func(ov_func)
                                                                                                                    , tmpl_arguments(tmpl_arguments)
{

}

CallInfo PartiallyInstantiatedFunctionSymbol::resolveCall(std::vector<ValueInfo> arguments) const 
{
    std::vector<VariableType> types;
    for ( auto arg : arguments )
        types.push_back(arg.type());

    if ( auto overload = ov_func -> overloadOfTemplateFunction(ov_func -> template_function, types, tmpl_arguments) )
    {
        if ( !ov_func -> checkValues(arguments, overload -> type().typeInfo().params()) )
            throw SemanticError("lvalue error");

        overload -> is_used = true;
        return CallInfo(overload, ov_func -> getConversions(arguments, overload -> type().typeInfo().params()));
    }
    else
        return ov_func -> resolveCall(arguments);
}
    
std::string PartiallyInstantiatedFunctionSymbol::getName() const 
{
    return ov_func -> getName();
}
