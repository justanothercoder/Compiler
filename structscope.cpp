#include "structscope.hpp"
#include "scopevisitor.hpp"
#include "symbol.hpp"
#include "variablesymbol.hpp"

StructScope::StructScope(std::string name
                         , Scope *enclosing_scope
                         , const TemplateInfo& template_info) : scope_name(enclosing_scope -> getScopeName() + "_" + name)
                                                              , enclosing_scope(enclosing_scope)
                                                              , template_info(template_info)
{
    type_size = 0;
}

Scope* StructScope::enclosingScope() const
{
    return enclosing_scope;
}

Symbol* StructScope::resolve(std::string name) const
{
    auto it = table.find(name);
    if ( it == std::end(table) )
    {
        if ( enclosingScope() )
            return enclosingScope() -> resolve(name);
        return nullptr;
    }
    return it -> second;
}

Symbol* StructScope::resolveMember(std::string name) const
{
    auto it = table.find(name);
    if ( it == std::end(table) )
        return nullptr;
    return it -> second;
}

void StructScope::accept(ScopeVisitor& visitor)
{
    visitor.visit(this);
}

std::string StructScope::getScopeName() const
{
    return scope_name;
}

const TemplateInfo& StructScope::templateInfo() const
{
    return template_info;
}
    
int StructScope::offsetOf(VariableSymbol* member) const
{
    int offset = 0;

    for ( auto entry : table )
    {
        if ( entry.second -> getSymbolType() == SymbolType::VARIABLE )
        {
            if ( entry.second == member )
                return offset;
            else
                offset += static_cast<VariableSymbol*>(entry.second) -> getType().sizeOf();
        }
    }

    throw std::logic_error("Not found");
}
