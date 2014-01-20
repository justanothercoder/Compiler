#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "globalconfig.hpp"

class OverloadedFunctionSymbol : public Symbol, public Type
{
public:

    OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info, bool is_method = false);

    virtual string getName() const;
    virtual int getSize() const;

    OverloadedFunctionTypeInfo getTypeInfo() const;

    void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym);

    bool isMethod() const;

    Type *getBaseType() const;

    virtual bool isReference() const;
    virtual SymbolType getSymbolType() const;
    
private:

    string name;
    OverloadedFunctionTypeInfo type_info;

    bool is_method;
};

#endif
