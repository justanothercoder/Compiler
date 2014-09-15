#include "functionscope.hpp"
#include "scopevisitor.hpp"

FunctionScope::FunctionScope(std::string scope_name, Scope *enclosing_scope) : scope_name(scope_name)
																			 , enclosing_scope(enclosing_scope)
																		     , template_info(enclosing_scope -> getTemplateInfo())
{

}

Scope* FunctionScope::getEnclosingScope() const 
{ 
	return enclosing_scope; 
}

std::string FunctionScope::getScopeName() const 
{ 
	return scope_name; 
}

void FunctionScope::accept(ScopeVisitor *visitor) 
{ 
	visitor -> visit(this); 
}
	
VarAllocator& FunctionScope::getVarAlloc() const 
{ 
	return var_alloc; 
}

TempAllocator& FunctionScope::getTempAlloc() const
{ 
	return temp_alloc; 
}

const TemplateInfo& FunctionScope::getTemplateInfo() const
{
	return template_info;
}

