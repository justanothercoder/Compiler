#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_ #include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "fieldvarallocator.hpp"

#include "basescope.hpp"

using std::map;

class StructSymbol : public Symbol, public BaseScope, public Type
{
public:

    friend class VariableSymbolDefine;

    void accept(ScopeVisitor *visitor) override;
    
    StructSymbol(string name, Scope *enclosing_scope);

    SymbolType getSymbolType() const override;
    string getName() const override;

    int getSize() const override;
    TypeKind getTypeKind() const override;
	
    Symbol* resolveMember(string name) const;

    Scope* getEnclosingScope() const override;
    Symbol* resolve(string name) const override;

    string getScopeName() const override;

	VarAllocator* get_valloc() override;

	FunctionSymbol* getConversionTo(StructSymbol *st);
	bool isConvertableTo(StructSymbol *st);

	int rankOfConversion(StructSymbol *st);

	FunctionSymbol* getConversionConstructor(StructSymbol *st);
	bool hasConversionConstructor(StructSymbol *st);

	FunctionSymbol* getConversionOperator(StructSymbol *st);
	bool hasConversionOperator(StructSymbol *st);

	FunctionSymbol* getCopyConstructor();
	FunctionSymbol* getDefaultConstructor();

	FunctionSymbol* constructorWith(FunctionTypeInfo ft);

	TempAllocator& getTempAlloc() override;

private:

    string name;
    
    Scope *enclosing_scope;
    
    int type_size;

    string scope_name;

	FieldVarAllocator valloc;
	TempAllocator temp_alloc;
};
		     
#endif
