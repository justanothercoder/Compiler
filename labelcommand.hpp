#ifndef _LABELCOMMAND_HPP_
#define _LABELCOMMAND_HPP_

#include "command.hpp"

class LabelCommand : public Command
{
public:

    LabelCommand(Arg* label);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;

private:

    Arg* label;
};

#endif
