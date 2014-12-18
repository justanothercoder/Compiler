#ifndef _LOCALSCOPE_HPP_
#define _LOCALSCOPE_HPP_

#include "basescope.hpp"

class LocalScope : public BaseScope
{
public:

    LocalScope(Scope *enclosing_scope);

    void accept(ScopeVisitor& visitor) override;

    Scope* enclosingScope() const override;
    std::string getScopeName() const override;

    const TemplateInfo& templateInfo() const override;
    bool isUnsafeBlock() const override;

private:

    Scope *enclosing_scope;
};

#endif
