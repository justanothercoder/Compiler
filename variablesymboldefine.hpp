#ifndef _VARIABLESYMBOLDEFINE_HPP_
#define _VARIABLESYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"
#include "variablesymbol.hpp"
#include "globalscope.hpp"
#include "localscope.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"

class VariableSymbolDefine : public ScopeVisitor
{
public:

    VariableSymbolDefine(VariableSymbol *sym);

    virtual void visit(GlobalScope *sc);
    virtual void visit(LocalScope *sc);
    virtual void visit(StructSymbol *sc);
    virtual void visit(FunctionSymbol *sc);

private:
    VariableSymbol *sym;
};

#endif
