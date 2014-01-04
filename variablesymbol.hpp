#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "type.hpp"

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(string name, Type *type);

    virtual Type* getType();

    void setType(Type *t);

    virtual Scope* getScope();
    virtual void setScope(Scope *scope);

    virtual string getName();

private:

    string name;

    Scope *symbol_scope;    

    Type *type;
};

#endif
