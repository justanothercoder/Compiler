#ifndef _DEFINESYMBOLVISITOR_HPP_
#define _DEFINESYMBOLVISITOR_HPP_

#include <memory>
#include "scopevisitor.hpp"

class Symbol;

class DefineSymbolVisitor : public ScopeVisitor
{
public:
    
    virtual void setSymbol(std::shared_ptr<const Symbol> sym) = 0;
};

#endif
