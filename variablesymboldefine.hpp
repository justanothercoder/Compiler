#ifndef _VARIABLESYMBOLDEFINE_HPP_
#define _VARIABLESYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"
#include "variablesymbol.hpp"

class VariableSymbolDefine : public ScopeVisitor
{
public:

    VariableSymbolDefine(VariableSymbol *sym);

    void visit(GlobalScope *sc) override;
    void visit(LocalScope *sc) override;
    void visit(StructScope *sc) override;
    void visit(FunctionSymbol *sc) override;

private:
    VariableSymbol *sym;
};

#endif
