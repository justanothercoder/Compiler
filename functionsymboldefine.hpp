#ifndef _FUNCTIONSYMBOLDEFINE_HPP_
#define _FUNCTIONSYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"

class BaseScope;

class FunctionSymbolDefine : public ScopeVisitor
{
public:

    FunctionSymbolDefine(FunctionSymbol *sym);

    void visit(GlobalScope *sc) override;
    void visit(LocalScope *sc) override;
    void visit(StructScope *sc) override;
    void visit(FunctionSymbol *sc) override;

	void visit(BaseScope *sc);

private:
    FunctionSymbol *sym;
};

#endif
