#include "basescope.hpp"
#include "symbol.hpp"
#include "functionalsymbol.hpp"
#include "varsymbol.hpp"
#include "typesymbol.hpp"
#include "builtintypesymbol.hpp"
#include "modulesymbol.hpp"
#include "templatesymbol.hpp"
#include "aliassymbol.hpp"
#include "functiontypeinfo.hpp"

#include "logger.hpp"

BaseScope::~BaseScope() { } 

Symbol* BaseScope::resolveHere(const std::string& name) const { return table.resolve(name); }

Symbol* BaseScope::resolve(const std::string& name) const
{
    auto symbol = resolveHere(name);
    if ( enclosingScope() == nullptr ) 
        return symbol;
    return symbol == nullptr ? enclosingScope() -> resolve(name) : symbol;
}
    
void BaseScope::define(std::unique_ptr<FunctionalSymbol> func) { func -> scope = this; table.define(std::move(func)); }
void BaseScope::define(std::unique_ptr<VarSymbol> var)         { var  -> scope = this; table.define(std::move(var));  }
void BaseScope::define(std::unique_ptr<TypeSymbol> type)       { type -> scope = this; table.define(std::move(type)); }
    
void BaseScope::define(std::unique_ptr<BuiltInTypeSymbol> builtin) { builtin -> scope = this; table.define(std::move(builtin)); }
void BaseScope::define(std::unique_ptr<AliasSymbol> alias)         { alias   -> scope = this; table.define(std::move(alias)); }
void BaseScope::define(std::unique_ptr<TemplateSymbol> templ)      { templ   -> scope = this; table.define(std::move(templ)); }

void BaseScope::define(std::shared_ptr<ModuleSymbol> module) { table.define(module); }

std::vector<VarSymbol*> BaseScope::getVars() const             { return table.getVars(); }
std::vector<FunctionalSymbol*> BaseScope::getFunctions() const { return table.getFunctions(); }
std::vector<Symbol*> BaseScope::allSymbols() const             { return table.allSymbols(); }

FunctionalSymbol* BaseScope::resolveFunction(const std::string& name, const FunctionTypeInfo& info) const 
{
    auto overloads = table.resolveFunction(name);
    
    for ( const auto& overload : overloads )
    {       
        if ( overload -> isCompatibleWith(info) )
            return overload;
    }

    if ( enclosingScope() )
        return enclosingScope() -> resolveFunction(name, info); 

    return nullptr;
}

