#ifndef _SYMBOLDEFINE_HPP_
#define _SYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"
#include "variablesymbol.hpp"

class SymbolDefine : public ScopeVisitor
{
public:

    SymbolDefine(Symbol *sym);

    virtual void visit(GlobalScope *sc);
    virtual void visit(LocalScope *sc);
    virtual void visit(StructScope *sc);
    virtual void visit(FunctionSymbol *sc);

private:
    Symbol *sym;
};

#endif
