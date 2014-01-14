#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>

using std::string;

class Type
{
public:

    virtual string getName() const = 0;
    virtual int getSize() const = 0;
};

#endif
