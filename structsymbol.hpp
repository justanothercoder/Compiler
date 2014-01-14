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

    virtual Scope* getEnclosingScope() const;
    virtual Symbol* resolve(string name) const;
    virtual void define(Symbol *sym);

    virtual string getName() const;
    virtual int getSize() const;

    virtual int getScopeSize() const;
	
    virtual string getScopeName() const;

    Symbol* resolveMember(string name) const;

private:

    string name;
    
    Scope *enclosing_scope;
    
    int type_size;

    string scope_name;
};
		     
#endif
