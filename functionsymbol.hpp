#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>
#include <boost/optional.hpp>

#include "basescope.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"

#include "overloadedfunctionsymbol.hpp"
#include "paramvarallocator.hpp"

#include "templateinfo.hpp"

#include "codeobject.hpp"

class VariableSymbol;

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
    friend class VariableSymbolDefine;
public:

    FunctionSymbol(std::string name, Type *return_type, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits, boost::optional<CodeObject> code_obj = boost::none);

    std::string getTypedName() const;
	std::string getScopedTypedName() const;
    
    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

	std::string getScopeName() const override;
    Scope* getEnclosingScope() const override;
    void accept(ScopeVisitor *visitor) override;

	std::string getName() const override;
	SymbolType getSymbolType() const override;
    
    FunctionTraits getTraits() const;
    
    TypeKind getTypeKind() const override;
    size_t getSize() const override;

	VarAllocator& getVarAlloc() const override;
	TempAllocator& getTempAlloc() const override;

	boost::optional<CodeObject> code_obj;

	Type *return_type;
    FunctionTypeInfo function_type_info;	
	
	bool is_constexpr;

	const TemplateInfo& getTemplateInfo() const override;
	bool isUnsafeBlock() const override;

	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *type) const override;

private:

    string name;
    
    Scope *enclosing_scope;

    string scope_name;

    FunctionTraits traits;

	TempAllocator temp_alloc;
	ParamVarAllocator var_alloc;

	TemplateInfo template_info;
};

#endif
