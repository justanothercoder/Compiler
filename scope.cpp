#include "scope.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "variablenode.hpp"
#include "typefactory.hpp"
#include "structdeclarationnode.hpp"

Scope::~Scope()
{

}

bool Scope::isUnsafeBlock() const
{
    return false;
}

void Scope::define(Symbol *sym)
{
    accept(sym -> getScopeVisitor());
}

const Type* Scope::resolveType(std::string name)
{
    auto scope = this;

    Symbol *sym = nullptr;

    while ( true )
    {
        while ( scope != nullptr && scope -> resolve(name) == sym )
            scope = scope -> getEnclosingScope();

        if ( scope == nullptr )
            return nullptr;

        sym = scope -> resolve(name);

        if ( dynamic_cast<Type*>(sym) != nullptr )
            return dynamic_cast<Type*>(sym);
    }
}

