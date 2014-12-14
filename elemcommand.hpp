#ifndef _ELEMCOMMAND_HPP_
#define _ELEMCOMMAND_HPP_

#include "command.hpp"

class ElemCommand : public Command
{
public:

    ElemCommand(Arg* base, Arg* expr, bool is_string = false);

    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;

private:

    Arg* base;
    Arg* expr;

    bool is_string;
};

#endif
