#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_ #include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "fieldvarallocator.hpp"

using std::map;

class StructSymbol : public Symbol, public BaseScope, public Type
{
public:

    friend class VariableSymbolDefine;

    virtual void accept(ScopeVisitor *visitor);
    
    StructSymbol(string name, Scope *enclosing_scope);

    virtual SymbolType getSymbolType() const;
    virtual string getName() const;

    virtual int getSize() const;
    virtual TypeKind getTypeKind() const ;
	
    Symbol* resolveMember(string name) const;

    virtual Scope* getEnclosingScope() const;
    virtual Symbol* resolve(string name) const;

    virtual string getScopeName() const;

	virtual VarAllocator* get_valloc() const;

	bool isConvertableTo(StructSymbol *st);

	bool hasConversionConstructor(StructSymbol *st);
	bool hasConversionOperator(StructSymbol *st);

	OverloadedFunctionSymbol* getOverloadedMethod(string name);

private:

    string name;
    
    Scope *enclosing_scope;
    
    int type_size;

    string scope_name;

	mutable FieldVarAllocator valloc;
};
		     
#endif
