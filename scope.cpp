#include "scope.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "variablenode.hpp"
#include "typefactory.hpp"
#include "structdeclarationnode.hpp"
#include "templatesymbol.hpp"
#include "definesymbolvisitor.hpp"

Scope::~Scope() { }
bool Scope::isUnsafeBlock() const { return false; }

void Scope::define(std::shared_ptr<const Symbol> sym) 
{
    auto define_symbol = sym -> defineSymbolVisitor();
    define_symbol -> setSymbol(sym);
    accept(*define_symbol); 
}

const Type* Scope::resolveType(const std::string& name)
{
    auto scope = this;

    const Symbol *sym = nullptr;

    while ( true )
    {
        while ( scope != nullptr && scope -> resolve(name) == sym )
            scope = scope -> enclosingScope();

        if ( scope == nullptr )
            return nullptr;

        sym = scope -> resolve(name);

        if ( sym -> getSymbolType() != SymbolType::OVERLOADED_FUNCTION && dynamic_cast<const Type*>(sym) != nullptr )
            return dynamic_cast<const Type*>(sym);
    }
}
    
const FunctionalType* Scope::resolveFunction(const std::string& name)
{
    auto func = resolve(name);
    assert(func -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);
    return static_cast<const OverloadedFunctionSymbol*>(func);
}
