#ifndef _VARIABLESYMBOLDEFINE_HPP_
#define _VARIABLESYMBOLDEFINE_HPP_

#include <memory>
#include "definesymbolvisitor.hpp"

class Symbol;

class VariableSymbolDefine : public DefineSymbolVisitor
{
public:

    void setSymbol(std::shared_ptr<const Symbol> sym) override;

    void visit(GlobalScope*   sc) override;
    void visit(LocalScope*    sc) override;
    void visit(StructScope*   sc) override;
    void visit(FunctionScope* sc) override;
    void visit(ModuleSymbol*  sc) override;

private:
    std::shared_ptr<const Symbol> sym;
};

#endif
