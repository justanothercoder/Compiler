#include "functionsymboldefine.hpp"
#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "modulesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

FunctionSymbolDefine::FunctionSymbolDefine(FunctionSymbol* sym) : sym(sym)
{

}

void FunctionSymbolDefine::visit(ModuleSymbol* sc)  { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(GlobalScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(LocalScope* sc)    { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(StructScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(FunctionScope* sc) { visit(static_cast<BaseScope*>(sc)); }

void FunctionSymbolDefine::visit(BaseScope* sc)
{
    std::string sym_name = sym -> getName();

    auto it = sc -> table.find(sym_name);

    if ( it == std::end(sc -> table) )
        sc -> table[sym_name] = new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym -> getTraits());

    auto _sym = sc -> table.at(sym_name);

    if ( _sym -> getSymbolType() != SymbolType::OVERLOADED_FUNCTION )
        throw SemanticError(sym_name + " is already defined.");

    auto ofs = static_cast<const OverloadedFunctionSymbol*>(_sym);
    ofs -> addOverload(sym -> type().typeInfo(), sym);
}
