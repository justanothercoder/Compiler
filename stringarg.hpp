#ifndef _STRINGARG_HPP_
#define _STRINGARG_HPP_

#include "arg.hpp"

class StringArg : public Arg
{
public:

    StringArg(std::string str);

    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;
    
    const Type* type() const override;

private:

    std::string str;
};

#endif
