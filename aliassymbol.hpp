#ifndef _ALIASSYMBOL_HPP_
#define _ALIASSYMBOL_HPP_

#include "symbol.hpp"

class AliasSymbol : public Symbol
{
public:

    AliasSymbol(const Symbol* aliased);
    
    std::string getName() const override;

    bool isVariable() const override;
    bool isType() const override;
    bool isFunction() const override;
    bool isModule() const override;

private:

    const Symbol* aliased;
};

#endif
