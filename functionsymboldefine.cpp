#include "functionsymboldefine.hpp"
#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "modulesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

void FunctionSymbolDefine::setSymbol(std::shared_ptr<const Symbol> sym) { this -> sym = sym; }

void FunctionSymbolDefine::visit(ModuleSymbol* sc)  { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(GlobalScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(LocalScope* sc)    { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(StructScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(FunctionScope* sc) { visit(static_cast<BaseScope*>(sc)); }

void FunctionSymbolDefine::visit(BaseScope* sc)
{
//    assert(sym.get() -> getSymbolType() == SymbolType::FUNCTION);
    auto function = static_cast<const FunctionSymbol*>(sym.get());

    auto sym_name = sym -> getName();

    auto it = sc -> table.find(sym_name);

    if ( it == std::end(sc -> table) )
        sc -> table[sym_name] = std::make_shared<OverloadedFunctionSymbol>(sym_name, function -> getTraits());

    auto _sym = sc -> table.at(sym_name);

    if ( !_sym -> isFunction() )
        throw SemanticError(sym_name + " is already defined.");

    auto ofs = static_cast<const OverloadedFunctionSymbol*>(_sym.get());
    ofs -> addOverload(function -> type().typeInfo(), sym);
}
