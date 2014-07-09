#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "alldeps.hpp"

#include "basescope.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

#include "varallocator.hpp"

using std::map;

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    void accept(ScopeVisitor *visitor) override;

    Scope* getEnclosingScope() const override;
    string getScopeName() const override;
	
	VarAllocator* get_valloc() override;
	TempAllocator& getTempAlloc() override;

private:

	VarAllocator valloc;
	TempAllocator temp_alloc;
};

#endif
