#ifndef _SPACEALLOCATOR_HPP_
#define _SPACEALLOCATOR_HPP_

#include <map>

class Command;
class VariableSymbol;

class SpaceAllocator
{
public:

    SpaceAllocator(int params_offset);

    void remember(const Command* command, int size);
    void remember(VariableSymbol* sym);

    void rememberAt(const Command* command, int address);

    int addressOf(const Command* command);
    int addressOf(VariableSymbol* sym);

    int totalSpaceUsed() const;

    std::string dumpDisposition() const;

private:

    std::map<const Command*, int> offsets;
    std::map<VariableSymbol*, int> var_offsets;

    int space_used;
    int param_space;    

    int space_offset;
    int params_offset;
};

#endif
