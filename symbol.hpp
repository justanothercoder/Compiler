#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>
#include <memory>
#include "semanticerror.hpp"

class Scope;

class Symbol
{
public:

    virtual ~Symbol();
    virtual std::string getName() const = 0;

    virtual bool isVariable() const;
    virtual bool isType() const;
    virtual bool isFunction() const;
    virtual bool isModule() const;

    bool is_unsafe;
    mutable bool is_used;
    Scope* scope;
};

#endif
