#ifndef _LABELARG_HPP_
#define _LABELARG_HPP_

#include "arg.hpp"

class LabelArg : public Arg
{
public:

    LabelArg(std::string label);

    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;
    
    const Type* type() const override;

private:

    std::string label;
};

#endif
