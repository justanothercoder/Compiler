#include "spaceallocator.hpp"
#include "globalconfig.hpp"
#include "variablesymbol.hpp"

SpaceAllocator::SpaceAllocator(int params_offset) : space_used (0)
                                                  , param_space(0)
                                                  , space_offset(GlobalConfig::int_size)
                                                  , params_offset(params_offset) 
{

}

void SpaceAllocator::remember(const Command* command, int size) 
{   
    offsets[command] = space_offset + space_used;
    space_used += size;    
}
    
void SpaceAllocator::rememberAt(const Command* command, int address)
{
    offsets[command] = address;
}

void SpaceAllocator::remember(VariableSymbol* sym)
{
    if ( var_offsets.count(sym) )
        return;

    if ( sym -> isParam() )
    {
        param_space += sym -> getType().sizeOf();
        var_offsets[sym] = -(params_offset + param_space);
    }
    else
    {
        var_offsets[sym] = space_offset + space_used;
        space_used += sym -> getType().sizeOf();
    }
}

int SpaceAllocator::addressOf(const Command* command) { return offsets.at(command); }
int SpaceAllocator::addressOf(VariableSymbol* sym)    { return var_offsets.at(sym); }

int SpaceAllocator::totalSpaceUsed() const { return space_used; }
