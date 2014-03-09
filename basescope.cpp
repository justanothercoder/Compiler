#include "basescope.hpp"

Symbol* BaseScope::resolve(string name) const
{
	auto it = table.find(name);
	if ( it == std::end(table) )
	{
		if ( getEnclosingScope() )
			return getEnclosingScope()->resolve(name);
		return nullptr;
	}
	return it->second;    
}
