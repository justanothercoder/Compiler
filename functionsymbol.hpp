#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "basescope.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"

#include "overloadedfunctionsymbol.hpp"

class VariableSymbol;

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
    friend class VariableSymbolDefine;
    
public:

    FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits);

    void setTypeInfo(FunctionTypeInfo function_type_info);

    string getTypedName() const;
    string getScopedTypedName() const;
    
    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    virtual int getScopeSize() const;
    virtual string getScopeName() const;
	virtual int getFreeAddress() const;
    virtual Scope* getEnclosingScope() const;
    virtual void accept(ScopeVisitor *visitor);

    void recalc_scope_address();

    virtual string getName() const;
    virtual SymbolType getSymbolType() const;
    
    FunctionTypeInfo getTypeInfo() const;
    FunctionTraits getTraits() const;
    
    virtual TypeKind getTypeKind() const;
    virtual int getSize() const;
    

private:

    int params_size;
    
    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    string scope_name;

    FunctionTraits traits;
};

#endif
