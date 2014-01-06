#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"

using std::map;

class StructSymbol : public Symbol, public BaseScope, public Type
{
public:
    
    StructSymbol(string name, Scope *enclosing_scope);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    virtual string getName();
    virtual int getSize();

    virtual int getScopeSize();
	
    virtual string getScopeName();

    virtual Scope* getScope();
    virtual void setScope(Scope *scope);    

    Symbol* resolveMember(string name);

private:

    string name;
    
    Scope *enclosing_scope;
    
    int type_size;

    string scope_name;

    Scope *symbol_scope;
};
		     
#endif
