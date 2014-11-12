#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>
#include <boost/optional.hpp>

#include "functionscope.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include "globaltable.hpp"

class VariableSymbol;

class FunctionSymbol : public Symbol, public FunctionScope, public Type
{
public:

    FunctionSymbol(std::string name, const Type *return_type, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits);

    std::string getTypedName() const;
    std::string getScopedTypedName() const;

    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    std::string getName() const override;
    SymbolType getSymbolType() const override;

    FunctionTraits getTraits() const;

    TypeKind getTypeKind() const override;
    size_t getSize() const override;

    const Type *return_type;
    FunctionTypeInfo function_type_info;

    bool is_constexpr;

    mutable bool is_used;

    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *type) const override;

    FunctionSymbol* getConversionTo(const Type *type) const override;

    const Symbol* getSymbol() const override;

    ScopeVisitor& getScopeVisitor() override;

    bool isUnsafeBlock() const override;

private:

    std::string name;

    FunctionTraits traits;
};

#endif
