#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "typedsymbol.hpp"
#include "functiontype.hpp"
#include "scope.hpp"

using std::map;
using std::pair;

class FunctionSymbol : public TypedSymbol, public Scope
{
public:

    FunctionSymbol(string name, FunctionType *function_type, Scope *enclosing_scope);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    virtual Type* getType();

    void setType(FunctionType *function_type);
    
private:

    FunctionType *function_type;

    Scope *enclosing_scope;
    map<string, Symbol*> members;
};

#endif
