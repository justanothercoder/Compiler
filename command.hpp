#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "arg.hpp"

class Command
{
public:
    virtual void gen(const Block& block, CodeObject& code_obj) const = 0;
    virtual std::string toString() const = 0;
    virtual bool isExpr() const = 0;
};

#endif
