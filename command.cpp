#include "command.hpp"

Command::Command(SSAOp op, Arg arg) : op(op), arg1(arg), arg2(IdType::NOID, -1)
{
    if (!(op == SSAOp::DEREF 
       || op == SSAOp::ADDR 
       || op == SSAOp::GOTO
       || op == SSAOp::LABEL
       || op == SSAOp::RETURN
       || op == SSAOp::NEW ))
        throw std::logic_error("Internal error.");
}

Command::Command(SSAOp op, Arg arg1, Arg arg2) : op(op), arg1(arg1), arg2(arg2)
{
    if (op == SSAOp::DEREF 
     || op == SSAOp::ADDR 
     || op == SSAOp::GOTO
     || op == SSAOp::LABEL
     || op == SSAOp::RETURN
     || op == SSAOp::NEW )
        throw std::logic_error("Internal error.");

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
