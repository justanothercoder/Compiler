#ifndef _SCOPEVISITOR_HPP_
#define _SCOPEVISITOR_HPP_

class GlobalScope;
class LocalScope;
class StructScope;
class FunctionScope;
class ModuleSymbol;

class ScopeVisitor
{
public:

    virtual void visit(GlobalScope*   sc) = 0;
    virtual void visit(LocalScope*    sc) = 0;
    virtual void visit(StructScope*   sc) = 0;
    virtual void visit(FunctionScope* sc) = 0;
    virtual void visit(ModuleSymbol*  sc) = 0;
};

#endif
