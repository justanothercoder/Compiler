#ifndef _SCOPEVISITOR_HPP_
#define _SCOPEVISITOR_HPP_

class GlobalScope;
class LocalScope;
class StructSymbol;
class FunctionSymbol;

class ScopeVisitor
{
public:

    virtual void visit(GlobalScope *sc) = 0;
    virtual void visit(LocalScope *sc) = 0;
    virtual void visit(StructSymbol *sc) = 0;
    virtual void visit(FunctionSymbol *sc) = 0;
	
};

#endif
