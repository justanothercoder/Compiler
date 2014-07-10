#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "basescope.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"

#include "overloadedfunctionsymbol.hpp"
#include "paramvarallocator.hpp"

#include "codeobject.hpp"
#include "optional.hpp"

class VariableSymbol;

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
    friend class VariableSymbolDefine;
public:

    FunctionSymbol(string name, VariableType return_type, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits, optional<CodeObject> code_obj = optional<CodeObject>::empty());

    string getTypedName() const;
    string getScopedTypedName() const;
    
    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    string getScopeName() const override;
    Scope* getEnclosingScope() const override;
    void accept(ScopeVisitor *visitor) override;

    string getName() const override;
	SymbolType getSymbolType() const override;
    
    FunctionTraits getTraits() const;
    
    TypeKind getTypeKind() const override;
    int getSize() const override;
	VarAllocator* get_valloc() override;

	TempAllocator& getTempAlloc() override;

	optional<CodeObject> code_obj;

	VariableType return_type;
    FunctionTypeInfo function_type_info;

private:

    string name;
    
    Scope *enclosing_scope;

    string scope_name;

    FunctionTraits traits;

	ParamVarAllocator valloc;

	TempAllocator temp_alloc;
};

#endif
