#include "variablesymboldefine.hpp"
#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "functionsymbol.hpp"
#include "modulesymbol.hpp"

void VariableSymbolDefine::setSymbol(std::shared_ptr<const Symbol> sym) { this -> sym = sym; }

void VariableSymbolDefine::visit(GlobalScope* sc)   { sc -> table[sym -> getName()] = sym; }
void VariableSymbolDefine::visit(LocalScope* sc)    { sc -> table[sym -> getName()] = sym; }
void VariableSymbolDefine::visit(FunctionScope* sc) { sc -> table[sym -> getName()] = sym; }
void VariableSymbolDefine::visit(ModuleSymbol* sc)  { sc -> table[sym -> getName()] = sym; }

void VariableSymbolDefine::visit(StructScope* sc)
{
    sc -> table[sym -> getName()] = sym;
    sc -> type_size += static_cast<const VariableSymbol*>(sym.get()) -> getType().sizeOf();
}
