#ifndef _FUNCTIONSCOPE_HPP_
#define _FUNCTIONSCOPE_HPP_

#include "basescope.hpp"
#include "templateinfo.hpp"

class FunctionSymbol;

class FunctionScope : public BaseScope
{
    friend class VariableSymbolDefine;

public:

    FunctionScope(std::string scope_name, Scope* enclosing_scope, bool is_unsafe);

    std::string getScopeName() const override;
    Scope* enclosingScope() const override;
    void accept(ScopeVisitor& visitor) override;

    const TemplateInfo& templateInfo() const override;
    bool isUnsafeBlock() const override;

    const FunctionSymbol* func;

    TemplateInfo template_info;

private:

    std::string scope_name;
    Scope* enclosing_scope;

    bool is_unsafe;
};

#endif
