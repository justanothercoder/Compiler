#ifndef _TEMPORARYARG_HPP_
#define _TEMPORARYARG_HPP_

#include "arg.hpp"

class Command;

class TemporaryArg : public Arg
{
    static int temp_id;

public:

    TemporaryArg(Command* command);
    
    std::string toString() const override;
    void gen(const Block& block, CodeObject& code_obj) const override;
    
    const Type* type() const override;

    Command* command;
};

#endif
