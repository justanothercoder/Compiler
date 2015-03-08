#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "structscope.hpp"
#include "symbolfactory.hpp"
#include "functiondeclarationnode.hpp"

#include "logger.hpp"

FunctionSymbol::FunctionSymbol(std::string name, FunctionType type, FunctionScope* scope, FunctionTraits traits, bool is_unsafe) 
    : name        (name)
    , traits_     (traits)
    , type_       (type)
    , scope       (scope)
{
    this -> is_unsafe = is_unsafe;
}

FunctionTraits FunctionSymbol::traits() const { return traits_; } 

std::string FunctionSymbol::getName() const { return name; }
std::string FunctionSymbol::typeName() const { return name; }
FunctionType FunctionSymbol::type() const { return type_; } 
Scope* FunctionSymbol::innerScope() const { return scope; }
    
bool FunctionSymbol::isCompatibleWith(FunctionTypeInfo ft) const 
{   
    return type_.typeInfo().isCompatibleWith(ft); 
}

CallInfo FunctionSymbol::resolveCall(std::vector<ValueInfo> arguments) const 
{
    if ( isMethod() )
        arguments.insert(std::begin(arguments), {type().typeInfo().paramAt(0), true});

    std::vector<VariableType> types;
    for ( auto arg : arguments )
        types.push_back(arg.type());

    auto function_params = type().typeInfo().params();

    if ( !checkValues(arguments, function_params) )
        throw SemanticError("lvalue error");

    is_used = true;
    return CallInfo(this, getConversions(arguments, function_params));
}

AST* FunctionSymbol::getFunctionBody() const { return function_body; }
    
bool FunctionSymbol::isFunction() const { return true; }
    
std::vector<VarSymbol*> FunctionSymbol::paramsSymbols() const
{    
    if ( scope == nullptr )
        return { };
    return scope -> paramsSymbols();
}
