#ifndef _NUMBERARG_HPP_
#define _NUMBERARG_HPP_

#include "arg.hpp"

class NumberArg : public Arg
{
public:    
    
    NumberArg(int _value);

    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;

    const Type* type() const override;

    int value() const;

private:

    int _value;
};

#endif
