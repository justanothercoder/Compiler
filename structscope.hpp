#ifndef _STRUCTSCOPE_HPP_
#define _STRUCTSCOPE_HPP_

#include "basescope.hpp"
#include "tempallocator.hpp"
#include "fieldvarallocator.hpp"
#include "templateinfo.hpp"

class StructScope : public BaseScope
{
    friend class VariableSymbolDefine;

public:

    StructScope(std::string scope_name, Scope *enclosing_scope, const TemplateInfo& template_info);

    VarAllocator& varAlloc() const override;
    TempAllocator& tempAlloc() const override;

    Scope* enclosingScope() const override;

    Symbol* resolveMember(std::string name) const;
    Symbol* resolve(std::string name) const override;

    std::string getScopeName() const override;

    void accept(ScopeVisitor& visitor) override;

    const TemplateInfo& templateInfo() const override;

private:
    mutable FieldVarAllocator var_alloc;
    mutable TempAllocator temp_alloc;

    std::string scope_name;
    Scope *enclosing_scope;

    TemplateInfo template_info;

protected:

    int type_size;
};

#endif
