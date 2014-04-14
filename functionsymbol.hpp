#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "basescope.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"

#include "overloadedfunctionsymbol.hpp"
#include "paramvarallocator.hpp"

class VariableSymbol;

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
    friend class VariableSymbolDefine;
public:

    FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits, CodeObject *code_obj = nullptr);

    void setTypeInfo(FunctionTypeInfo function_type_info);

    string getTypedName() const;
    string getScopedTypedName() const;
    
    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    virtual string getScopeName() const;
    virtual Scope* getEnclosingScope() const;
    virtual void accept(ScopeVisitor *visitor);

    virtual string getName() const;
    virtual SymbolType getSymbolType() const;
    
    FunctionTypeInfo getTypeInfo() const;
    FunctionTraits getTraits() const;
    
    virtual TypeKind getTypeKind() const;
    virtual int getSize() const;
	virtual VarAllocator* get_valloc() const;

	CodeObject *code_obj;

private:

    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    string scope_name;

    FunctionTraits traits;

	mutable ParamVarAllocator valloc;
};

#endif
