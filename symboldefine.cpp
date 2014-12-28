#include "symboldefine.hpp"

#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "functionsymbol.hpp"
#include "modulesymbol.hpp"
#include "logger.hpp"

SymbolDefine::SymbolDefine(Symbol* sym) : sym(sym)
{

}

void SymbolDefine::visit(BaseScope* sc)
{
    if ( sc -> table.find(sym -> getName()) != std::end(sc -> table) )
        throw SemanticError("Error: " + sym -> getName() + " is already defined");

    sc -> table[sym -> getName()] = sym;
}

void SymbolDefine::visit(GlobalScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void SymbolDefine::visit(LocalScope* sc)    { visit(static_cast<BaseScope*>(sc)); }
void SymbolDefine::visit(StructScope* sc)   { visit(static_cast<BaseScope*>(sc)); } 
void SymbolDefine::visit(FunctionScope* sc) { visit(static_cast<BaseScope*>(sc)); }
void SymbolDefine::visit(ModuleSymbol* sc)  { visit(static_cast<BaseScope*>(sc)); }

