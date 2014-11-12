#include "tempallocator.hpp"

#include "logger.hpp"

TempAllocator::TempAllocator() : space_needed(0), space_used(0)
{

}

void TempAllocator::add(int space)
{
    space_needed += space;
}

void TempAllocator::claim(int space)
{
    stack.push(space);
    space_used += space;
}

void TempAllocator::free()
{
    space_used -= stack.top();
    stack.pop();
}

int TempAllocator::getOffset() const
{
    return space_used;
}

int TempAllocator::getSpaceNeeded() const
{
    return space_needed;
}
