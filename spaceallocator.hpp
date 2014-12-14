#ifndef _SPACEALLOCATOR_HPP_
#define _SPACEALLOCATOR_HPP_

class Command;

class SpaceAllocator
{
public:

    void remember(const Command* command, int offset);
    int addressOf(const Command* command);

private:

};

#endif
