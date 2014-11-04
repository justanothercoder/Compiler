#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <stdexcept>
#include "arg.hpp"

enum class SSAOp
{
    PLUS, MINUS, MUL, DIV, MOD, ELEM, DEREF, ADDR, ASSIGN, PARAM, CALL, 
    LABEL, RETURN, IF, IFFALSE, GOTO, EQUALS, NEQUALS, DOT, NEW, RETURNREF
};

class Type;

struct Command
{
    Command(SSAOp op, Arg arg);
    Command(SSAOp op, Arg arg1, Arg arg2);

//	std::string toString() const;

    bool isExpr() const;

    bool operator==(const Command& c) const;

    SSAOp op;
    Arg arg1;
    Arg arg2;
};

namespace std
{
template<>
struct hash<Command>
{
    typedef Command argument_type;
    typedef std::size_t result_type;

    result_type operator() (const argument_type& s) const
    {
        hash<Arg> h;
        return static_cast<int>(s.op) + (h(s.arg1) << 32) + h(s.arg2);
    }
};
}

#endif
