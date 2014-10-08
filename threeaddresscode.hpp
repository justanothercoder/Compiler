#ifndef _THREEADRESSCODE_HPP_
#define _THREEADRESSCODE_HPP_

#include <string>
#include <vector>
#include <stack>

#include "globalhelper.hpp"
#include "functionsymbol.hpp"
#include "codeobject.hpp"

/* list of SSA commands
 *
 * I use three address code (TAC) as low-level IR.
 * It will be implemented with indirect triples.
 *
 * +------------------+------+------+---------+
 * |      command     | arg1 | arg2 |   op    |
 * +------------------+------+------+---------+
 * |      a + b       |  a   |  b   |    +    |
 * +------------------+------+------+---------+
 * |      a - b       |  a   |  b   |    -    |
 * +------------------+------+------+---------+
 * |      a * b       |  a   |  b   |    *    |
 * +------------------+------+------+---------+
 * |      a / b       |  a   |  b   |    /    |
 * +------------------+------+------+---------+
 * |      a % b       |  a   |  b   |    %    |
 * +------------------+------+------+---------+
 * |      a == b      |  a   |  b   |   ==    |
 * +------------------+------+------+---------+
 * |       a[i]       |  a   |  i   |   []    |
 * +------------------+------+------+---------+
 * |        *a        |  a   |      |   (*)   |
 * +------------------+------+------+---------+
 * |        &a        |  a   |      |    &    |
 * +------------------+------+------+---------+
 * |      a = b       |  a   |  b   |    =    |
 * +------------------+------+------+---------+
 * |     param a      |  a   |      |  param  |
 * +------------------+------+------+---------+
 * |     call p n     |  p   |  n   |  call   |
 * +------------------+------+------+---------+
 * |      goto L      |  L   |      |  goto   |
 * +------------------+------+------+---------+
 * |   if x goto L    |  x   |  L   |   if    |
 * +------------------+------+------+---------+
 * | ifFalse x goto L |  x   |  L   | ifFalse |
 * +------------------+------+------+---------+
 * |        a.b       |  a   |  b   |    .    |
 * +------------------+------+------+---------+
 */

enum class SSAOp { PLUS, MINUS, MUL, DIV, MOD, ELEM, DEREF, ADDR, ASSIGN, PARAM, CALL, LABEL, RETURN, IF, IFFALSE, GOTO, EQUALS, DOT };
enum class IdType { NOID, NUMBER, STRING, VARIABLE, TEMP, LABEL, PROCEDURE };

struct Arg
{
	Arg(IdType type, int id) : type(type), id(id)
	{

	}

	std::string toString()
	{
		switch ( type )
		{
		case IdType::NOID     : return "noid";
		case IdType::STRING   : return '"' + GlobalHelper::id_to_string[id] + '"';
		case IdType::NUMBER   : return std::to_string(GlobalHelper::id_to_num[id]);
		case IdType::TEMP     : return "temp_" + std::to_string(id);
		case IdType::LABEL    : return GlobalHelper::label_name[id] + ":";
		case IdType::PROCEDURE: return GlobalHelper::func_by_id[id] -> getScopedTypedName();
		case IdType::VARIABLE : return "var_" + std::to_string(id);
		}
	}

	IdType type;
	int id;
};

struct Command
{
	Command(SSAOp op, Arg arg1, Arg arg2 = Arg(IdType::NOID, -1)) : op(op), arg1(arg1), arg2(arg2) 
	{

	} 

	std::string toString()
	{
		switch ( op )
		{
		case SSAOp::ASSIGN : return arg1.toString() + " = "  + arg2.toString();
		case SSAOp::PLUS   : return arg1.toString() + " + "  + arg2.toString();
		case SSAOp::MINUS  : return arg1.toString() + " - "  + arg2.toString();
		case SSAOp::MUL    : return arg1.toString() + " * "  + arg2.toString();
		case SSAOp::DIV    : return arg1.toString() + " / "  + arg2.toString();
		case SSAOp::MOD    : return arg1.toString() + " % "  + arg2.toString();
		case SSAOp::EQUALS : return arg1.toString() + " == " + arg2.toString();
		case SSAOp::ELEM   : return arg1.toString() + "[" + arg2.toString() + "]";
		case SSAOp::DOT    : return arg1.toString() + "." + arg2.toString();
		case SSAOp::DEREF  : return "*" + arg1.toString();
		case SSAOp::ADDR   : return "&" + arg1.toString();
		case SSAOp::PARAM  : return "param " + arg1.toString();
		case SSAOp::CALL   : return "call " + arg1.toString() + ' ' + std::to_string(arg2.id);
		case SSAOp::LABEL  : return arg1.toString();
		case SSAOp::RETURN : return "return " + arg1.toString();
		case SSAOp::IF     : return "if " + arg1.toString() + " goto " + arg2.toString();
		case SSAOp::IFFALSE: return "ifFalse " + arg1.toString() + " goto " + arg2.toString();
		case SSAOp::GOTO   : return "goto " + arg1.toString(); 
		default: throw std::logic_error("not all SSAOp catched in toString");
		}
	}

	SSAOp op;
	Arg arg1;
	Arg arg2;
};

class ThreeAddressCode
{
public:

	Arg add(Command command);
	Arg newTemp();
	Arg newLabel(std::string label = "");

	std::string toString();

	void pushScope(VarAllocator alloc);
	void popScope();

	CodeObject genAsm() const;

private:

	std::stack<VarAllocator> var_alloc;
	std::stack<TempAllocator> temp_alloc;

	std::string genArg(Arg arg) const;
	std::string genCommand(Command command) const;

	std::vector<Command> commands;
	std::vector<int> code;

};

#endif
