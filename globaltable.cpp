#include "globaltable.hpp"
#include "scope.hpp"
#include "varallocator.hpp"
#include "globalconfig.hpp"

int GlobalTable::transformAddress(Scope *scope, int addr)
{
	return scope -> getVarAlloc().getSpace() + GlobalConfig::int_size + addr; 
}
