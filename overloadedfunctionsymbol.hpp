#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "symbol.hpp"
#include "templateargument.hpp"
#include "functiontraits.hpp"
#include "functionalsymbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"

class TemplateSymbol;
class TemplateFunctionSymbol;

class OverloadedFunctionSymbol : public FunctionalSymbol
{
    friend class PartiallyInstantiatedFunctionSymbol;

public:

    OverloadedFunctionSymbol(std::string name, FunctionTraits traits);
    std::string getName() const override;
    std::string typeName() const override;

    FunctionalSymbol* overloadWith(FunctionTypeInfo info) const;
    void addOverload(FunctionTypeInfo type_info, FunctionalSymbol* sym) const;

    std::vector<FunctionalSymbol*> allOverloads() const;

    bool isFunction() const override;

    const TemplateFunctionSymbol* templateFunction() const;
    void setTemplateFunction(const TemplateFunctionSymbol* function) const;
    
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;
    AST* getFunctionDecl() const override;

    Scope* innerScope() const;
    
    FunctionType type() const override;
    FunctionTraits traits() const override;
    
    bool isCompatibleWith(FunctionTypeInfo ft) const override; 

private:

    VariableType getBaseType() const;

    FunctionalSymbol* getViableOverload(FunctionTypeInfo params_type) const;

private:

    std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits_;
    mutable const TemplateFunctionSymbol* template_function = nullptr;
};

#endif
