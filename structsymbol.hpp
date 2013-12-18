#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "symbol.hpp"
#include "scope.hpp"

using std::map;

class StructSymbol : public Symbol, Scope
{
public:
    
    StructSymbol(string name, Scope *enclosing_scope);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

private:

    map<string, Symbol*> members;
    
    Scope *enclosing_scope;
};
		     
#endif
