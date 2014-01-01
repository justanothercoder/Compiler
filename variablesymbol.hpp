#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "typedsymbol.hpp"
#include "type.hpp"

class VariableSymbol : public TypedSymbol
{
public:

    VariableSymbol(string name, Type *type);

    virtual Type* getType();

    void setType(Type *t);

private:

    Type *type;
};

#endif
