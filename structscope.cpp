#include "structscope.hpp"
#include "scopevisitor.hpp"

StructScope::StructScope(std::string name
		               , Scope *enclosing_scope
					   , const TemplateInfo& template_info) : scope_name(enclosing_scope -> getScopeName() + "_" + name)
															, enclosing_scope(enclosing_scope)
															, template_info(template_info)
{
	type_size = 0;
}

TempAllocator& StructScope::getTempAlloc() const
{
	return temp_alloc;
}
	
VarAllocator& StructScope::getVarAlloc() const
{
   	return var_alloc; 
}
	
Scope* StructScope::getEnclosingScope() const 
{ 
	return enclosing_scope; 
}

Symbol* StructScope::resolve(std::string name) const
{
	auto it = table.find(name);
	if ( it == std::end(table) )
	{
		if ( getEnclosingScope() )
			return getEnclosingScope() -> resolve(name);
		return nullptr;
	}
	return it -> second;
}

Symbol* StructScope::resolveMember(std::string name) const
{
	auto it = table.find(name);
	if ( it == std::end(table) )
		return nullptr;
	return it -> second;    
}

void StructScope::accept(ScopeVisitor *visitor) 
{
   	visitor -> visit(this); 
}

std::string StructScope::getScopeName() const 
{ 
	return scope_name; 
}
	
const TemplateInfo& StructScope::getTemplateInfo() const
{
	return template_info;
}
