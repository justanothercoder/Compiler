#ifndef _IFCOMMAND_HPP_
#define _IFCOMMAND_HPP_

#include "command.hpp"

class IfCommand : public Command
{
public:

    IfCommand(Arg* expr, Arg* label_false);
    
    void gen(Block& block, CodeObject& code_obj) override;
    std::string toString() override;
    bool isExpr() const override;

private:

    Arg* expr;
    Arg* label_false;
};

#endif
