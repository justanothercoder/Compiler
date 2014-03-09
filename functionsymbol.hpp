#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "basescope.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"

#include "overloadedfunctionsymbol.hpp"
#include "varallocator.hpp"

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
    virtual Scope* getEnclosingScope() const;
    virtual void accept(ScopeVisitor *visitor);


    virtual string getName() const;
    virtual SymbolType getSymbolType() const;
    
    FunctionTypeInfo getTypeInfo() const;
    FunctionTraits getTraits() const;
    
    virtual TypeKind getTypeKind() const;
    virtual int getSize() const;

	virtual void increaseScopeTreeSize(int size);

	int getParamsSize() const;

	virtual VarAllocator* get_valloc() const;

private:

    int params_size;
    
    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    string scope_name;

    FunctionTraits traits;

	mutable VarAllocator valloc;
};

#endif
