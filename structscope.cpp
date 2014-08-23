#include "structscope.hpp"
#include "scopevisitor.hpp"

StructScope::StructScope(string name, Scope *enclosing_scope) : scope_name(enclosing_scope -> getScopeName() + "_" + name)
															  , enclosing_scope(enclosing_scope)	
{
	type_size = 0;
}

TempAllocator& StructScope::getTempAlloc() 
{
	return temp_alloc;
}
	
VarAllocator& StructScope::getVarAlloc() 
{
   	return var_alloc; 
}
	
Scope* StructScope::getEnclosingScope() const 
{ 
	return enclosing_scope; 
}

Symbol* StructScope::resolve(string name) const
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

Symbol* StructScope::resolveMember(string name) const
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

string StructScope::getScopeName() const 
{ 
	return scope_name; 
}
	
const TemplateInfo& StructScope::getTemplateInfo() const
{
	return template_info;
}

bool StructScope::isUnsafeBlock() const
{
	return false;
}
    
