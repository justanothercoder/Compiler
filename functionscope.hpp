#ifndef _FUNCTIONSCOPE_HPP_
#define _FUNCTIONSCOPE_HPP_

#include "basescope.hpp"
#include "tempallocator.hpp"
#include "paramvarallocator.hpp"
#include "templateinfo.hpp"

class FunctionSymbol;

class FunctionScope : public BaseScope
{
    friend class VariableSymbolDefine;

public:

    FunctionScope(std::string scope_name, Scope *enclosing_scope, bool is_constr, bool is_unsafe);

    std::string getScopeName() const override;
    Scope* getEnclosingScope() const override;
    void accept(ScopeVisitor& visitor) override;

    VarAllocator& getVarAlloc() const override;
    TempAllocator& getTempAlloc() const override;

    const TemplateInfo& templateInfo() const override;

    bool isUnsafeBlock() const override;

    FunctionSymbol *func;

private:

    std::string scope_name;
    Scope *enclosing_scope;

    mutable TempAllocator temp_alloc;
    mutable ParamVarAllocator var_alloc;

    TemplateInfo template_info;

    bool is_unsafe;
};

#endif
