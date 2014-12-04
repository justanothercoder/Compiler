#include "command.hpp"

#include <cassert>

Command::Command(SSAOp op, Arg arg) : op(op), arg1(arg), arg2(IdType::NOID, -1), offset(0)
{
    assert(op == SSAOp::DEREF 
        || op == SSAOp::ADDR 
        || op == SSAOp::GOTO
        || op == SSAOp::LABEL
        || op == SSAOp::RETURN
        || op == SSAOp::RETURNREF
        || op == SSAOp::NEW);
}

Command::Command(SSAOp op, Arg arg1, Arg arg2) : op(op), arg1(arg1), arg2(arg2), offset(0)
{
    assert(!(op == SSAOp::DEREF 
     || op == SSAOp::ADDR 
     || op == SSAOp::GOTO
     || op == SSAOp::LABEL
     || op == SSAOp::RETURN
     || op == SSAOp::RETURNREF
     || op == SSAOp::NEW ));
}

bool Command::isExpr() const
{
    switch ( op )
    {
    case SSAOp::GOTO: case SSAOp::IFFALSE: case SSAOp::IF:
    case SSAOp::RETURN: case SSAOp::RETURNREF: case SSAOp::LABEL:
    case SSAOp::PARAM: case SSAOp::ASSIGN: case SSAOp::ASSIGNCHAR:
        return false;
    default:
        return true;
    }
}

bool Command::operator==(const Command& c) const
{
    return op == c.op && arg1 == c.arg1 && arg2 == c.arg2;
}
