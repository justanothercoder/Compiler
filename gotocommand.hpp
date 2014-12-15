#ifndef _GOTOCOMMAND_HPP_
#define _GOTOCOMMAND_HPP_

#include "command.hpp"

class GotoCommand : public Command
{
public:

    GotoCommand(Arg* label);

    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

private:
    
    Arg* label;
};

#endif
