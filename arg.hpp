#ifndef _ARG_HPP_
#define _ARG_HPP_

#include <string>

class Type;
struct Block;
class CodeObject;

class Arg
{
public:
    virtual std::string toString() const = 0;
    virtual void gen(const Block& block, CodeObject& code_obj) const = 0;

    virtual const Type* type() const = 0;
};

#endif
