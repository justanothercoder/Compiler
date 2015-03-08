#include "functionscope.hpp"
#include "varsymbol.hpp"

FunctionScope::FunctionScope(std::string scope_name
                           , Scope *enclosing_scope
                           , bool is_unsafe) : scope_name     (scope_name)
                                             , enclosing_scope(enclosing_scope)
                                             , is_unsafe      (is_unsafe)
{

}

Scope* FunctionScope::enclosingScope() const { return enclosing_scope; }
std::string FunctionScope::getScopeName() const { return scope_name; }

bool FunctionScope::isUnsafeBlock() const { return is_unsafe; }
    
std::vector<VarSymbol*> FunctionScope::paramsSymbols() const
{
    return params_symbols;
}
    
void FunctionScope::define(std::unique_ptr<VarSymbol> var) 
{
    if ( var -> isParam() )
        params_symbols.push_back(var.get());
    BaseScope::define(std::move(var));
}
