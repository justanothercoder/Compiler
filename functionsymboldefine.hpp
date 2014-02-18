#ifndef _FUNCTIONSYMBOLDEFINE_HPP_
#define _FUNCTIONSYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"
#include "variablesymbol.hpp"
#include "globalscope.hpp"
#include "localscope.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"

class FunctionSymbolDefine : public ScopeVisitor
{
public:

    FunctionSymbolDefine(FunctionSymbol *sym);

    virtual void visit(GlobalScope *sc);
    virtual void visit(LocalScope *sc);
    virtual void visit(StructSymbol *sc);
    virtual void visit(FunctionSymbol *sc);

	void visit(BaseScope *sc);

private:
    FunctionSymbol *sym;
};

#endif
