#include "basescope.hpp"
#include "symbol.hpp"

BaseScope::~BaseScope()
{
    for ( auto name_and_sym : table )
        delete name_and_sym.second;
}

Symbol* BaseScope::resolveHere(std::string name) const
{
    auto it = table.find(name);
    return it == std::end(table) ? nullptr : it -> second;
}

Symbol* BaseScope::resolve(std::string name) const
{
    auto symbol = resolveHere(name);
    if ( enclosingScope() == nullptr ) 
        return symbol;
    return symbol == nullptr ? enclosingScope() -> resolve(name) : symbol;
}
