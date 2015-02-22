#ifndef _VARIABLEARG_HPP_
#define _VARIABLEARG_HPP_

#include "arg.hpp"

class VarSymbol;

class VariableArg : public Arg
{
public:

    VariableArg(const VarSymbol* var);
    
    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;

    const Type* type() const override;
    const VarSymbol* var() const;

private:

    const VarSymbol* var_;
};

#endif
