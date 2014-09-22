#include "globalscope.hpp"
#include "scopevisitor.hpp"

GlobalScope::GlobalScope() : BaseScope(), template_info(TemplateInfo())
{

} 

Scope* GlobalScope::getEnclosingScope() const 
{
   	return nullptr; 
}

std::string GlobalScope::getScopeName() const
{
   	return ""; 
}

void GlobalScope::accept(ScopeVisitor& visitor)
{
   	visitor.visit(this); 
}

VarAllocator& GlobalScope::getVarAlloc() const 
{
   	return var_alloc; 
}

TempAllocator& GlobalScope::getTempAlloc() const
{
   	return temp_alloc; 
}

const TemplateInfo& GlobalScope::getTemplateInfo() const
{ 
	return template_info;
}

bool GlobalScope::isUnsafeBlock() const
{
	return false;
}
