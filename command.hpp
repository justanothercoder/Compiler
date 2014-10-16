#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <stdexcept>
#include "arg.hpp"

enum class SSAOp { PLUS, MINUS, MUL, DIV, MOD, ELEM, DEREF, ADDR, ASSIGN, PARAM, CALL, LABEL, RETURN, IF, IFFALSE, GOTO, EQUALS, DOT };

struct Command
{
	Command(SSAOp op, Arg arg1, Arg arg2 = Arg(IdType::NOID, -1));

	std::string toString();

    bool isExpr() const;

	SSAOp op;
	Arg arg1;
	Arg arg2;
};


#endif
