#include "variablesymboldefine.hpp"

#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "functionsymbol.hpp"

#include "globaltable.hpp"

VariableSymbolDefine::VariableSymbolDefine(VariableSymbol *sym) : sym(sym)
{

}

void VariableSymbolDefine::visit(GlobalScope *sc)
{
    sc -> table[sym -> getName()] = sym;
    sc -> getVarAlloc().addVariable(sym);
}

void VariableSymbolDefine::visit(LocalScope *sc)
{
    sc -> table[sym -> getName()] = sym;
    sc -> getVarAlloc().addVariable(sym);
}

void VariableSymbolDefine::visit(StructScope *sc)
{
    sc -> table[sym -> getName()] = sym;
    sc -> getVarAlloc().addVariable(sym);
    sc -> type_size += sym -> getType() -> getSize();
}

void VariableSymbolDefine::visit(FunctionScope *sc)
{
    sc -> table[sym -> getName()] = sym;
    sc -> getVarAlloc().addVariable(sym);
}
