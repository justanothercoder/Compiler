#ifndef _DOTCOMMAND_HPP_
#define _DOTCOMMAND_HPP_

#include "command.hpp"

class VariableSymbol;

class DotCommand : public Command
{
public:

    DotCommand(Arg* expr, int offset, VariableSymbol* member);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* expr;
    int offset;
    VariableSymbol* member;
};

#endif
