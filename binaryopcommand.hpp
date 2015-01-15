#ifndef _BINARYOPCOMMAND_HPP_
#define _BINARYOPCOMMAND_HPP_

#include <memory>
#include "command.hpp"
#include "binaryoperatornode.hpp"

class BinaryOpCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    BinaryOpCommand(BinaryOp op, Argument lhs, Argument rhs);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* lhs();
    Arg* rhs();

    BinaryOp op();

private:

    BinaryOp op_;

    Argument lhs_;
    Argument rhs_;
};

#endif
