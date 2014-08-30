#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>
#include <boost/optional.hpp>

#include "basescope.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"
#include "paramvarallocator.hpp"
#include "templateinfo.hpp"
#include "codeobject.hpp"

class VariableSymbol;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
    friend class VariableSymbolDefine;
public:

    FunctionSymbol(std::string name, const Type *return_type, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits, boost::optional<CodeObject> code_obj = boost::none);

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

	const Type *return_type;
    FunctionTypeInfo function_type_info;	
	
	bool is_constexpr;

	const TemplateInfo& getTemplateInfo() const override;
	bool isUnsafeBlock() const override;

	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *type) const override;
	
	bool isReference() const override;
	bool isConst() const override;

	FunctionSymbol* getConversionTo(const Type *type) const override;

	const Symbol* getSymbol() const override;

private:

	std::string name;
    
    Scope *enclosing_scope;

	std::string scope_name;

    FunctionTraits traits;

	mutable TempAllocator temp_alloc;
	mutable ParamVarAllocator var_alloc;

	TemplateInfo template_info;
};

#endif
