#ifndef _TEMPALLOCATOR_HPP_
#define _TEMPALLOCATOR_HPP_

#include <stack>

class TempAllocator
{
public:
	
	TempAllocator();

	void add(int space);

	void claim(int space);
	void free();

	int getOffset() const;

	int getSpaceNeeded() const;
		
private:

	int space_needed;
	int space_used;

	std::stack<int> stack;
};

#endif
