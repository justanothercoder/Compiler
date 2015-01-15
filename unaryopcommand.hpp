#ifndef _UNARYOPCOMMAND_HPP_
#define _UNARYOPCOMMAND_HPP_

#include <boost/variant.hpp>
#include "command.hpp"
#include "addrnode.hpp"
#include "unarynode.hpp"

class UnaryOpCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    UnaryOpCommand(AddrOp op , Argument expr);
    UnaryOpCommand(UnaryOp op, Argument expr);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* expr();
    boost::variant<AddrOp, UnaryOp> op();

private:

    boost::variant<AddrOp, UnaryOp> op_;
    Argument expr_;
};

#endif
