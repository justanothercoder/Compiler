#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "scope.hpp"

using std::map;

class GlobalScope : public Scope
{
public:

    GlobalScope();

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

private:

    map<string, Symbol*> table;
};

#endif
