#ifndef _LOCALSCOPE_HPP_
#define _LOCALSCOPE_HPP_

#include "basescope.hpp"

class LocalScope : public BaseScope
{
public:

    LocalScope(Scope* enclosing_scope);

    Scope* enclosingScope() const override;
    std::string getScopeName() const override;

    bool isUnsafeBlock() const override;

private:

    Scope* enclosing_scope;
};

#endif
