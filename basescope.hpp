#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>

#include "scope.hpp"
#include "variablesymbol.hpp"

using std::map;

class ExprNode;

class BaseScope : public Scope
{
public:

    friend class VariableSymbolDefine;
    friend class FunctionSymbolDefine;
    friend class SymbolDefine;
    
    BaseScope();

    virtual Symbol* resolve(string name) const;

    virtual int getAddress(VariableSymbol *sym) const;

	virtual int getScopeTreeSize() const;
	virtual void increaseScopeTreeSize(int size);

    virtual int getScopeSize() const;
    virtual int getScopeAddress() const;

public:
    map<string, Symbol*> table;
    map<VariableSymbol*, int> addresses;

    int scope_size, scope_address, scope_tree_size;
};

#endif
