#ifndef _STRUCTSCOPE_HPP_
#define _STRUCTSCOPE_HPP_

#include "basescope.hpp"
#include "templateinfo.hpp"

class StructScope : public BaseScope
{
    friend class VariableSymbolDefine;

public:

    StructScope(std::string scope_name, Scope *enclosing_scope, const TemplateInfo& template_info);

    Scope* enclosingScope() const override;

    Symbol* resolveMember(std::string name) const;
    Symbol* resolve(std::string name) const override;

    std::string getScopeName() const override;

    void accept(ScopeVisitor& visitor) override;

    const TemplateInfo& templateInfo() const override;

    int offsetOf(VariableSymbol* member) const;

private:
    
    std::string scope_name;
    Scope *enclosing_scope;

    TemplateInfo template_info;

protected:

    int type_size;
};

#endif
