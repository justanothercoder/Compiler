#include "spaceallocator.hpp"
#include "globalconfig.hpp"
#include "variablesymbol.hpp"
#include "command.hpp"

SpaceAllocator::SpaceAllocator(int params_offset) : params_offset(params_offset) { }

void SpaceAllocator::remember(const Command* command, int size) 
{   
    space_used += size;    
    offsets[command] = space_offset + space_used;
}
    
void SpaceAllocator::rememberAt(const Command* command, int address)
{
    offsets[command] = address;
}

void SpaceAllocator::remember(const VariableSymbol* sym)
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
        space_used += sym -> getType().sizeOf();
        var_offsets[sym] = space_offset + space_used;
    }
}

int SpaceAllocator::addressOf(const Command* command)    { return offsets.at(command); }
int SpaceAllocator::addressOf(const VariableSymbol* sym) { return var_offsets.at(sym); }

int SpaceAllocator::totalSpaceUsed() const { return space_used; }

std::string SpaceAllocator::dumpDisposition() const
{
    std::string dump = "";

    for ( auto var : var_offsets )
        dump += std::to_string(var.second) + ": " + var.first -> getName() + '\n';

    for ( auto temp : offsets )
        dump += std::to_string(temp.second) + ": " + temp.first -> toString() + '\n';

    return dump;
}
