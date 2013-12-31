#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "variablesymbol.hpp"

using std::map;

class StructSymbol : public Symbol, public Scope, public Type
{
public:
    
    StructSymbol(string name, Scope *enclosing_scope);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    virtual string getName();
    virtual int getSize();

private:

    map<string, Symbol*> members;
    
    Scope *enclosing_scope;

    int type_size;
};
		     
#endif
