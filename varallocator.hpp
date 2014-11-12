#ifndef _VARALLOCATOR_HPP_
#define _VARALLOCATOR_HPP_

#include <iostream>
#include <map>

class VariableSymbol;

class VarAllocator
{
public:
    VarAllocator();

    virtual void addVariable(VariableSymbol *var);

    int getAddress(VariableSymbol *var) const;

    int getSpace() const;

protected:

    int space_for_variables;

    std::map<VariableSymbol*, int> var_addresses;
};

#endif
