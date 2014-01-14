#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>

#include "semanticerror.hpp"

using std::string;

class Scope;

class Symbol
{
public:

    virtual ~Symbol();
    
    virtual string getName() const = 0;
    virtual Scope* getScope() const = 0;
    virtual void setScope(Scope *scope) = 0;    
};

#endif
