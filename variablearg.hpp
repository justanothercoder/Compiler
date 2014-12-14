#ifndef _VARIABLEARG_HPP_
#define _VARIABLEARG_HPP_

#include "arg.hpp"

class VariableSymbol;

class VariableArg : public Arg
{
public:

    VariableArg(VariableSymbol* var);
    
    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;

    const Type* type() const override;

    VariableSymbol* var;

};

#endif
