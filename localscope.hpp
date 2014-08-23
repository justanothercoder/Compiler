#ifndef _LOCALSCOPE_HPP_
#define _LOCALSCOPE_HPP_

#include "basescope.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"

#include "globalscope.hpp"

class LocalScope : public BaseScope
{
public:

    LocalScope(Scope *enclosing_scope);

    void accept(ScopeVisitor *visitor) override;

    Scope* getEnclosingScope() const override;
    string getScopeName() const override;

	VarAllocator& getVarAlloc() override;
	TempAllocator& getTempAlloc() override;

	const TemplateInfo& getTemplateInfo() const override;
	bool isUnsafeBlock() const override;
	
private:

    Scope *enclosing_scope;
};

#endif
