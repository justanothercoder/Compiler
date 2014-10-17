#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <stdexcept>
#include "arg.hpp"

enum class SSAOp { PLUS, MINUS, MUL, DIV, MOD, ELEM, DEREF, ADDR, ASSIGN, PARAM, CALL, LABEL, RETURN, IF, IFFALSE, GOTO, EQUALS, DOT };

class Type;

struct Command
{
	Command(SSAOp op, Arg arg1, Arg arg2 = Arg(IdType::NOID, -1));
	Command(Type *type, SSAOp op, Arg arg1, Arg arg2 = Arg(IdType::NOID, -1));

	std::string toString();

    bool isExpr() const;

    bool operator==(const Command& c) const;

	SSAOp op;
	Arg arg1;
	Arg arg2;

    Type *type;
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
            return reinterpret_cast<std::size_t>(&s);
        }
    };
}

#endif
