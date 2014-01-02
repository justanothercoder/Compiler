#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>

#include "semanticerror.hpp"

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
