#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>

#include "semanticerror.hpp"

class Scope;
class ScopeVisitor;

enum class SymbolType { FUNCTION, OVERLOADED_FUNCTION, VARIABLE, STRUCT, BUILTINTYPE, TEMPLATESTRUCT, CLASSVARIABLE, MODULE, TEMPLATEFUNCTION };

class Symbol
{
public:

    virtual ~Symbol();
    virtual std::string getName() const = 0;
    virtual SymbolType getSymbolType() const = 0;

    virtual ScopeVisitor& getScopeVisitor();

    bool is_unsafe;
};

#endif
