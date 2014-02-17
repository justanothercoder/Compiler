#ifndef _CLASSVARIABLESYMBOL_HPP_
#define _CLASSVARIABLESYMBOL_HPP_

#include "structsymbol.hpp"

class ClassVariableSymbol : public Type
{
public:

    ClassVariableSymbol(StructSymbol *sym);

    virtual string getName() const;
    virtual int getSize() const;

    virtual TypeKind getTypeKind() const;

    StructSymbol *sym;
};

#endif
