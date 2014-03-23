#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>

#include "semanticerror.hpp"

using std::string;

class Scope;

enum class SymbolType { FUNCTION, OVERLOADED_FUNCTION, VARIABLE, STRUCT, BUILTINTYPE, TEMPLATESTRUCT, CLASSVARIABLE };

class Symbol
{
public:

    virtual ~Symbol();    
    virtual string getName() const = 0;
    virtual SymbolType getSymbolType() const = 0;
};

#endif
