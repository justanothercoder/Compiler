#ifndef _DOTARG_HPP_
#define _DOTARG_HPP_

#include "arg.hpp"

class VariableSymbol;

class DotArg : public Arg
{
public:

    DotArg(Arg* expr, int offset, VariableSymbol* member);
    
    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;

    const Type* type() const override;

    Arg* expr;
    int offset;
    VariableSymbol* member;

};

#endif
