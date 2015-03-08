#ifndef _FUNCTIONSCOPE_HPP_
#define _FUNCTIONSCOPE_HPP_

#include "basescope.hpp"
#include "templateinfo.hpp"

class FunctionalSymbol;

class FunctionScope : public BaseScope
{
public:

    FunctionScope(std::string scope_name, Scope* enclosing_scope, bool is_unsafe);

    std::string getScopeName() const override;
    Scope* enclosingScope() const override;

    bool isUnsafeBlock() const override;
    std::vector<VarSymbol*> paramsSymbols() const;

    const FunctionalSymbol* func = nullptr;
    
    void define(std::unique_ptr<VarSymbol> var) override;

    TemplateInfo template_info;

private:

    std::string scope_name;
    Scope* enclosing_scope;

    bool is_unsafe;

    std::vector<VarSymbol*> params_symbols;
};

#endif
