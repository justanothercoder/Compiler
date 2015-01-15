#ifndef _SYMBOLDEFINE_HPP_
#define _SYMBOLDEFINE_HPP_

#include <memory>
#include "definesymbolvisitor.hpp"

class Symbol;
class GlobalScope;
class LocalScope;
class StructScope;
class FunctionSymbol;

class BaseScope;

class SymbolDefine : public DefineSymbolVisitor
{
public:

    void setSymbol(std::shared_ptr<const Symbol> sym) override;

    void visit(GlobalScope*   sc) override;
    void visit(LocalScope*    sc) override;
    void visit(StructScope*   sc) override;
    void visit(FunctionScope* sc) override;
    void visit(ModuleSymbol*  sc) override;

    void visit(BaseScope* sc);

private:
    std::shared_ptr<const Symbol> sym;
};

#endif
