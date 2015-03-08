#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>
#include <boost/optional.hpp>
#include "symbol.hpp"
#include "functionscope.hpp"
#include "functionalsymbol.hpp"

class VariableSymbol;
class AST;

class FunctionSymbol : public FunctionalSymbol
{
public:

    FunctionSymbol(std::string name, FunctionType type, FunctionScope* scope, FunctionTraits traits, bool is_unsafe = false);

    FunctionType type() const;
    Scope* innerScope() const override;

    std::string getName() const override;
    std::string typeName() const override;

    bool isFunction() const override;

    bool isCompatibleWith(FunctionTypeInfo ft) const override;
    FunctionTraits traits() const override;
    
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

    AST* getFunctionBody() const override;
    std::vector<VarSymbol*> paramsSymbols() const override;
    bool is_constexpr = false;

    AST* function_body = nullptr;
private:

    std::string name;

    FunctionTraits traits_;
    FunctionType type_;
    FunctionScope* scope;
};

#endif
