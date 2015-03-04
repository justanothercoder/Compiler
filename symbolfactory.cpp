#include "symbolfactory.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"

#include "builtins.hpp"

std::unique_ptr<VarSymbol> SymbolFactory::makeVariable(std::string name, VariableType type, VariableSymbolType var_sym_type)
{
    return std::make_unique<VariableSymbol>(name, type, var_sym_type);
}

std::unique_ptr<FunctionalSymbol> SymbolFactory::makeFunction(std::string name, FunctionType type, FunctionTraits traits, bool is_unsafe, FunctionScope* scope, AST* body)
{
    auto sym = std::make_unique<FunctionSymbol>(name, type, scope, traits, is_unsafe);
    if ( scope ) 
        scope -> func = sym.get();
    sym -> function_body = body;
    return std::move(sym);
}

std::unique_ptr<TypeSymbol> SymbolFactory::makeStruct(std::string name, StructScope* scope)
{
    return std::make_unique<StructSymbol>(name, scope);
}
    
std::unique_ptr<TypeSymbol> SymbolFactory::makeLambda(const std::vector<VarSymbol*>& capture, const std::vector<VarInfo>& formal_params, AST* body)
{
    static int lambda_class_num = 0;
    auto struct_name = "$_" + std::to_string(lambda_class_num++);

    auto struct_scope = new StructScope(struct_name, body -> scope.get());
    auto lambda_type = makeStruct(struct_name, struct_scope);
    
    auto params = std::vector<VariableType>{ };
    for ( auto param : formal_params )
        params.emplace_back(param.type());

    auto function_body = body -> copyTree().release();
    auto scope = std::make_shared<FunctionScope>(struct_scope -> getScopeName() + "_" + Comp::config().getCodeOperatorName("operator()"), struct_scope, false);

    for ( auto param : formal_params )
        scope -> define(makeVariable(param.name(), param.type(), VariableSymbolType::PARAM));

    function_body -> scope = scope;

    auto call_op = makeFunction("operator()", FunctionType(BuiltIns::void_type, params), FunctionTraits::method(), false, scope.get(), function_body);
    lambda_type -> defineMethod(std::move(call_op));

    return std::move(lambda_type);
}
