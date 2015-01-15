#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>
#include <memory>
#include "semanticerror.hpp"

class Scope;
class DefineSymbolVisitor;

enum class SymbolType { FUNCTION, OVERLOADED_FUNCTION, VARIABLE, STRUCT, BUILTINTYPE, TEMPLATESTRUCT, CLASSVARIABLE, MODULE, TEMPLATEFUNCTION };

class Symbol
{
public:

    virtual ~Symbol();
    virtual std::string getName() const = 0;
    virtual SymbolType getSymbolType() const = 0;

    virtual std::unique_ptr<DefineSymbolVisitor> defineSymbolVisitor() const;

    bool is_unsafe;
};

#endif
