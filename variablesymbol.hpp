#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"
#include "type.hpp"

enum class VariableSymbolType { SIMPLE, PARAM, FIELD };

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(string name, Type *type, VariableSymbolType sym_type = VariableSymbolType::SIMPLE);

    virtual Type* getType();

    void setType(Type *t);

    virtual Scope* getScope();
    virtual void setScope(Scope *scope);

    virtual string getName();

    bool isParam();
    bool isField();
       
private:

    string name;

    Scope *symbol_scope;    

    Type *type;

    VariableSymbolType sym_type;
};

#endif
