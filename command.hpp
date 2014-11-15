#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <stdexcept>
#include "arg.hpp"

enum class SSAOp
{
    PLUS, MINUS, MUL, DIV, MOD, ELEM, DEREF, ADDR, ASSIGN, PARAM, CALL, 
    LABEL, RETURN, IF, IFFALSE, GOTO, EQUALS, NEQUALS, DOT, NEW, RETURNREF,
    ASSIGNCHAR
};

class Type;

struct Command
{
    Command(SSAOp op, Arg arg);
    Command(SSAOp op, Arg arg1, Arg arg2);

    bool isExpr() const;

    bool operator==(const Command& c) const;

    SSAOp op;
    Arg arg1;
    Arg arg2;

    mutable int offset;
};

#endif
