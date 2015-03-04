#ifndef _FUNCTIONALSYMBOL_HPP_
#define _FUNCTIONALSYMBOL_HPP_

#include "scopedsymbol.hpp"
#include "functionaltype.hpp"
#include "functiontraits.hpp"
#include "functiontype.hpp"

class FunctionalSymbol : public ScopedSymbol, public FunctionalType
{
public:

    std::string getTypedName() const;
    std::string getScopedTypedName() const;

    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    virtual FunctionType type() const = 0;
    virtual FunctionTraits traits() const = 0;
    virtual bool isCompatibleWith(FunctionTypeInfo ft) const = 0; 

    virtual AST* getFunctionBody() const = 0;
};

#endif
