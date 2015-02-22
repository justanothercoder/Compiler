#ifndef _DOTARG_HPP_
#define _DOTARG_HPP_

#include <memory>
#include "arg.hpp"

class VarSymbol;

class DotArg : public Arg
{
public:

    DotArg(Argument expr, int offset, const VarSymbol* member);
    
    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;

    const Type* type() const override;

    Arg* expr() const;
    int offset() const;
    const VarSymbol* member() const;

private:

    Argument expr_;
    int offset_;
    const VarSymbol* member_;

};

#endif
