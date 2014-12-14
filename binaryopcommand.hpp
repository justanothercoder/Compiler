#ifndef _BINARYOPCOMMAND_HPP_
#define _BINARYOPCOMMAND_HPP_

#include "command.hpp"
#include "binaryoperatornode.hpp"

class BinaryOpCommand : public Command
{
public:

    BinaryOpCommand(BinaryOp op, Arg* lhs, Arg* rhs);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;

private:

    BinaryOp op;

    Arg* lhs;
    Arg* rhs;
};

#endif
