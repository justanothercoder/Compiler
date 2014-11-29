#ifndef _MODULESYMBOL_HPP_
#define _MODULESYMBOL_HPP_

#include <map>
#include "symbol.hpp"
#include "basescope.hpp"

class ModuleSymbol : public Symbol, public BaseScope
{
    friend class VariableSymbolDefine; 

public:
    ModuleSymbol(std::string name, Scope* enclosing_scope);

    std::string getName() const override;
    SymbolType getSymbolType() const override;
    
    Scope* getEnclosingScope() const override;

    VarAllocator& getVarAlloc() const override;
    TempAllocator& getTempAlloc() const override;

    std::string getScopeName() const override;

    void accept(ScopeVisitor& visitor) override;

    const TemplateInfo& getTemplateInfo() const override;

private:

    std::string name;
    Scope* enclosing_scope;
};

#endif
