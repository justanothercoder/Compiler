#ifndef _SYMBOLDEFINE_HPP_
#define _SYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"

class Symbol;
class GlobalScope;
class LocalScope;
class StructScope;
class FunctionSymbol;

class SymbolDefine : public ScopeVisitor
{
public:

    SymbolDefine(Symbol *sym);

    void visit(GlobalScope*   sc) override;
    void visit(LocalScope*    sc) override;
    void visit(StructScope*   sc) override;
    void visit(FunctionScope* sc) override;
    void visit(ModuleSymbol*  sc) override;

private:
    Symbol *sym;
};

#endif
