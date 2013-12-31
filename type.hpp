#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>

using std::string;

class Type
{
public:

    virtual string getName() = 0;
    virtual int getSize() = 0;
};

#endif