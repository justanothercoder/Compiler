#ifndef _TYPESYMBOL_HPP_
#define _TYPESYMBOL_HPP_

#include "scopedsymbol.hpp"
#include "objecttype.hpp"

class TypeSymbol : public ScopedSymbol, public ObjectType
{
public:

    virtual void defineMethod(std::unique_ptr<FunctionalSymbol> method) = 0;
    virtual void defineMember(std::unique_ptr<VarSymbol> member) = 0;
    
    const Symbol*           resolveMember(const std::string& name) const;
    const FunctionalSymbol* resolveMethod(const std::string& name) const;
};

#endif
