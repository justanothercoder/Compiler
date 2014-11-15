#include "functionsymboldefine.hpp"

#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

#include "globaltable.hpp"
#include "logger.hpp"

FunctionSymbolDefine::FunctionSymbolDefine(FunctionSymbol *sym) : sym(sym)
{

}

void FunctionSymbolDefine::visit(GlobalScope *sc)
{
    visit(static_cast<BaseScope*>(sc));
}

void FunctionSymbolDefine::visit(LocalScope *sc)
{
    visit(static_cast<BaseScope*>(sc));
}

void FunctionSymbolDefine::visit(StructScope *sc)
{
    visit(static_cast<BaseScope*>(sc));
}

void FunctionSymbolDefine::visit(FunctionScope *sc)
{
    visit(static_cast<BaseScope*>(sc));
}

void FunctionSymbolDefine::visit(BaseScope *sc)
{
    std::string sym_name = sym -> getName();

    auto it = sc -> table.find(sym_name);

    if ( it == std::end(sc -> table) )
        sc -> table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym -> getTraits()));

    auto _sym = sc -> table.at(sym_name);

    if ( _sym -> getSymbolType() != SymbolType::VARIABLE )
        throw SemanticError(sym_name + " is already defined.");

    if ( static_cast<VariableSymbol*>(_sym) -> getType() -> getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
        throw SemanticError(sym_name + " is already defined as not function");

    auto func_type_info = sym -> getType() -> getTypeInfo();

    Logger::log("symbol: " + sym -> getName() + " " + func_type_info.toString());

    auto ofs = static_cast<const OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(_sym) -> getType());
    ofs -> addOverload(func_type_info, sym);
}
