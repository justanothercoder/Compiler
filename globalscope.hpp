#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "basescope.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

#include "codegen.hpp"
#include "globalconfig.hpp"

using std::map;

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    virtual void accept(ScopeVisitor *visitor);

    virtual Scope* getEnclosingScope() const;
    virtual string getScopeName() const;
};

#endif
