#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>

using std::string;

class Symbol
{
public:

    virtual ~Symbol();
    
    Symbol(string name);

    virtual string getName();
    
protected:

    string name;
    
};

#endif
