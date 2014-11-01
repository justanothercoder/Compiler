#include "command.hpp"

Command::Command(SSAOp op, Arg arg1, Arg arg2) : op(op), arg1(arg1), arg2(arg2)
{

}

bool Command::isExpr() const
{
    switch ( op )
    {
    case SSAOp::GOTO  :
    case SSAOp::IFFALSE:
    case SSAOp::IF   :
    case SSAOp::RETURN:
    case SSAOp::LABEL  :
    case SSAOp::PARAM:
    case SSAOp::ASSIGN:
        return false;
    default:
        return true;
    }
}

bool Command::operator==(const Command& c) const
{
    return op == c.op && arg1 == c.arg1 && arg2 == c.arg2;
}
