#include "spaceallocator.hpp"

void SpaceAllocator::remember(const Command* command, int offset)
{
    offsets[command] = offset;
}

int SpaceAllocator::addressOf(const Command* command)
{
//    return offsets.at(command);
    return offsets[command];
}

int SpaceAllocator::totalSpaceUsed() const
{
    return 0;
}
