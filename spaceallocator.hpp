#ifndef _SPACEALLOCATOR_HPP_
#define _SPACEALLOCATOR_HPP_

#include <map>

class Command;

class SpaceAllocator
{
public:

    void remember(const Command* command, int offset);
    int addressOf(const Command* command);

    int totalSpaceUsed() const;

private:

    std::map<const Command*, int> offsets;

};

#endif
