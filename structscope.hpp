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
	
	StructScope(string scope_name, Scope *enclosing_scope, const TemplateInfo& template_info);
	
	VarAllocator& getVarAlloc() const override;
	TempAllocator& getTempAlloc() const override;

    Scope* getEnclosingScope() const override;
	
    Symbol* resolveMember(string name) const;
    Symbol* resolve(string name) const override;

    string getScopeName() const override;

    void accept(ScopeVisitor *visitor) override;
	
	const TemplateInfo& getTemplateInfo() const override;
	bool isUnsafeBlock() const override;
    
private:
	FieldVarAllocator var_alloc;
	TempAllocator temp_alloc;

	string scope_name;
	Scope *enclosing_scope;

	TemplateInfo template_info;

protected:

    int type_size;
};

#endif
