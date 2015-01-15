#ifndef _DOTARG_HPP_
#define _DOTARG_HPP_

#include <memory>
#include "arg.hpp"

class VariableSymbol;

class DotArg : public Arg
{
public:

    DotArg(Argument expr, int offset, const VariableSymbol* member);
    
    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;

    const Type* type() const override;

    Arg* expr() const;
    int offset() const;
    const VariableSymbol* member() const;

private:

    Argument expr_;
    int offset_;
    const VariableSymbol* member_;

};

#endif
