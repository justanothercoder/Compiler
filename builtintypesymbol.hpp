#ifndef _BUILTINTYPESYMBOL_HPP_
#define _BUILTINTYPESYMBOL_HPP_

#include "type.hpp"
#include "symbol.hpp"

class BuiltInTypeSymbol : public Symbol, public Type
{
public:

    BuiltInTypeSymbol(string name, int size);
    
    virtual string getName() const;
    virtual int getSize() const;

    virtual Scope* getScope() const;
    virtual void setScope(Scope *scope);    

private:
    
    string name;
    int size;
    Scope *symbol_scope;
    
};

#endif
