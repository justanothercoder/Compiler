#include "basescope.hpp"
#include "symbol.hpp"
	
BaseScope::~BaseScope()
{
	for ( auto name_and_sym : table )
		delete name_and_sym.second;
}

Symbol* BaseScope::resolve(std::string name) const
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
