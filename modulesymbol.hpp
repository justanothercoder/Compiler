#ifndef _MODULESYMBOL_HPP_
#define _MODULESYMBOL_HPP_

#include <map>
#include "symbol.hpp"
#include "basescope.hpp"

class ModuleSymbol : public Symbol, public BaseScope
{
public:

    ModuleSymbol(std::string name, Scope* enclosing_scope);

    std::string getName() const override;
        
    Scope* enclosingScope() const override;
    std::string getScopeName() const override;

    bool isModule() const override;

private:

    std::string name;
    Scope* enclosing_scope;
};

#endif
