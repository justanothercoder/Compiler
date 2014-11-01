#ifndef _UNSAFESCOPE_HPP_
#define _UNSAFESCOPE_HPP_

#include "localscope.hpp"

class UnsafeScope : public LocalScope
{
public:

    UnsafeScope(Scope *enclosing_scope);

    bool isUnsafeBlock() const override;
};

#endif
