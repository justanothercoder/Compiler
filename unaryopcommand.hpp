#ifndef _UNARYOPCOMMAND_HPP_
#define _UNARYOPCOMMAND_HPP_

#include <boost/variant.hpp>
#include "command.hpp"
#include "addrnode.hpp"
#include "unarynode.hpp"

class UnaryOpCommand : public Command
{
public:

    UnaryOpCommand(AddrOp op, Arg* expr);
    UnaryOpCommand(UnaryOp op, Arg* expr);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

private:

    boost::variant<AddrOp, UnaryOp> op;
    Arg* expr;
};

#endif
