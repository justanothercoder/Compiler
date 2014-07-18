#ifndef _SCOPEVISITOR_HPP_
#define _SCOPEVISITOR_HPP_

class GlobalScope;
class LocalScope;
class StructScope;
class FunctionSymbol;

class ScopeVisitor
{
public:

    virtual void visit(GlobalScope *sc) = 0;
    virtual void visit(LocalScope *sc) = 0;
    virtual void visit(StructScope *sc) = 0;
    virtual void visit(FunctionSymbol *sc) = 0;
	
};

#endif
