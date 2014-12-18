#ifndef _IFFALSECOMMAND_HPP_
#define _IFFALSECOMMAND_HPP_

#include "command.hpp"
#include "labelarg.hpp"

class IfFalseCommand : public Command
{
public:

    IfFalseCommand(Arg* expr, Arg* label_false);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    
    void accept(CommandVisitor* visitor) override;

    Arg* expr;
    Arg* label_false;
};

#endif
