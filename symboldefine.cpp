#include "symboldefine.hpp"

#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "functionsymbol.hpp"
#include "modulesymbol.hpp"

SymbolDefine::SymbolDefine(Symbol* sym) : sym(sym)
{

}

void SymbolDefine::visit(GlobalScope* sc)
{
    if ( sc -> table.find(sym -> getName()) != std::end(sc -> table) )
        throw SemanticError("Error: " + sym -> getName() + " is already defined");

    sc -> table[sym -> getName()] = sym;
}

void SymbolDefine::visit(LocalScope* sc)
{
    if ( sc -> table.find(sym -> getName()) != std::end(sc -> table) )
        throw SemanticError("Error: " + sym -> getName() + " is already defined");

    sc -> table[sym -> getName()] = sym;
}

void SymbolDefine::visit(StructScope* sc)
{
    if ( sc -> table.find(sym -> getName()) != std::end(sc -> table) )
        throw SemanticError("Error: " + sym -> getName() + " is already defined");

    sc -> table[sym -> getName()] = sym;
}

void SymbolDefine::visit(FunctionScope* sc)
{
    if ( sc -> table.find(sym -> getName()) != std::end(sc -> table) )
        throw SemanticError("Error: " + sym -> getName() + " is already defined");

    sc -> table[sym -> getName()] = sym;
}

void SymbolDefine::visit(ModuleSymbol* sc)
{
    if ( sc -> table.find(sym -> getName()) != std::end(sc -> table) )
        throw SemanticError("Error: " + sym -> getName() + " is already defined");

    sc -> table[sym -> getName()] = sym;
}
