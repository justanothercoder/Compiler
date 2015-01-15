#ifndef _FUNCTIONSYMBOLDEFINE_HPP_
#define _FUNCTIONSYMBOLDEFINE_HPP_

#include <memory>
#include "definesymbolvisitor.hpp"

class BaseScope;
class FunctionSymbol;

class FunctionSymbolDefine : public DefineSymbolVisitor
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
