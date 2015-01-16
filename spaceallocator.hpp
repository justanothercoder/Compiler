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
    void remember(const VariableSymbol* sym);

    void rememberAt(const Command* command, int address);

    int addressOf(const Command* command);
    int addressOf(const VariableSymbol* sym);

    int totalSpaceUsed() const;

    std::string dumpDisposition() const;

private:

    std::map<const Command*, int> offsets;
    std::map<const VariableSymbol*, int> var_offsets;

    int space_used  = 0;
    int param_space = 0;    

    int space_offset = 0;
    int params_offset;
};

#endif
