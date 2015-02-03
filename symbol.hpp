#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>
#include <memory>
#include "semanticerror.hpp"

class Scope;
class DefineSymbolVisitor;

class Symbol
{
public:

    virtual ~Symbol();
    virtual std::string getName() const = 0;

    virtual bool isVariable() const;
    virtual bool isType() const;
    virtual bool isFunction() const;
    virtual bool isModule() const;

    virtual std::unique_ptr<DefineSymbolVisitor> defineSymbolVisitor() const;

    bool is_unsafe;
};

#endif
