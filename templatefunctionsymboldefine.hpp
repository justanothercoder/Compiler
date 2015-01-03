#ifndef _TEMPLATEFUNCTIONSYMBOLDEFINE_HPP_
#define _TEMPLATEFUNCTIONSYMBOLDEFINE_HPP_

#include "scopevisitor.hpp"

class BaseScope;
class TemplateFunctionSymbol;

class TemplateFunctionSymbolDefine : public ScopeVisitor
{
public:

    TemplateFunctionSymbolDefine(TemplateFunctionSymbol *sym);

    void visit(GlobalScope*   sc) override;
    void visit(LocalScope*    sc) override;
    void visit(StructScope*   sc) override;
    void visit(FunctionScope* sc) override;
    void visit(ModuleSymbol*  sc) override;

    void visit(BaseScope* sc);

private:
    TemplateFunctionSymbol* sym;
};

#endif
