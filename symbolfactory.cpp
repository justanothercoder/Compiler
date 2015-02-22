#include "symbolfactory.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

std::unique_ptr<VarSymbol> SymbolFactory::makeVariable(std::string name, VariableType type, VariableSymbolType var_sym_type)
{
    return std::make_unique<VariableSymbol>(name, type, var_sym_type);
}

std::unique_ptr<FunctionalSymbol> SymbolFactory::makeFunction(std::string name, FunctionType type, FunctionTraits traits, bool is_unsafe, FunctionScope* scope, FunctionDeclarationNode* decl)
{
    auto sym = std::make_unique<FunctionSymbol>(name, type, scope, traits, is_unsafe);
    if ( scope ) 
        scope -> func = sym.get();
    sym -> function_decl = decl;
    return std::move(sym);
}

std::unique_ptr<TypeSymbol> SymbolFactory::makeStruct(std::string name, StructScope* scope)
{
    return std::make_unique<StructSymbol>(name, scope);
}
