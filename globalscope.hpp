#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "basescope.hpp"
#include "varallocator.hpp"
#include "tempallocator.hpp"
#include "templateinfo.hpp"

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    void accept(ScopeVisitor *visitor) override;

    Scope* getEnclosingScope() const override;
	std::string getScopeName() const override;
	
	VarAllocator& getVarAlloc() const override;
	TempAllocator& getTempAlloc() const override;

	const TemplateInfo& getTemplateInfo() const override;
	bool isUnsafeBlock() const override;
	
private:

	mutable VarAllocator var_alloc;
	mutable TempAllocator temp_alloc;

	TemplateInfo template_info;
};

#endif
