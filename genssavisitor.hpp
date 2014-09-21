#ifndef _GENSSAVISITOR_HPP_
#define _GENSSAVISITOR_HPP_

#include <vector>
#include <string>
#include "astvisitor.hpp"
#include "globalhelper.hpp"
#include "functionsymbol.hpp"

/* list of SSA commands
 *
 * I use three address code (TAC) as low-level IR.
 * It will be implemented with indirect triples.
 *
 * +-----------+------+------+-------+
 * |  command  | arg1 | arg2 |  op   |
 * +-----------+------+------+-------+
 * |   a + b   |  a   |  b   |   +   |
 * +-----------+------+------+-------+
 * |   a - b   |  a   |  b   |   -   |
 * +-----------+------+------+-------+
 * |   a * b   |  a   |  b   |   *   |
 * +-----------+------+------+-------+
 * |   a / b   |  a   |  b   |   /   |
 * +-----------+------+------+-------+
 * |   a % b   |  a   |  b   |   %   |
 * +-----------+------+------+-------+
 * |   a[i]    |  a   |  i   |  []   |
 * +-----------+------+------+-------+
 * |   *a      |  a   |      |  (*)  |
 * +-----------+------+------+-------+
 * |   &a      |  a   |      |   &   |
 * +-----------+------+------+-------+
 * |   a = b   |  a   |  b   |   =   |
 * +-----------+------+------+-------+
 * |  param a  |  a   |      | param |
 * +-----------+------+------+-------+
 * | call p n  |  p   |  n   | call  |
 * +-----------+------+------+-------+
 */

enum class SSAOp { PLUS, MINUS, MUL, DIV, MOD, ELEM, DEREF, ADDR, ASSIGN, PARAM, CALL, LABEL, RETURN };
enum class IdType { NOID, NUMBER, STRING, VARIABLE, TEMP, COMMAND, LABEL, PROCEDURE };

struct Arg
{
	Arg(IdType type, int id) : type(type), id(id)
	{

	}

	std::string toString()
	{
		if ( type == IdType::NUMBER )
			return std::to_string(GlobalHelper::id_to_num[id]);
		else if ( type == IdType::TEMP )
			return "temp_" + std::to_string(id);
		else if ( type == IdType::COMMAND )
			return "command_" + std::to_string(id);
		else if ( type == IdType::LABEL )
			return GlobalHelper::label_name[id] + ":";
		else if ( type == IdType::PROCEDURE )
			return GlobalHelper::func_by_id[id] -> getScopedTypedName();
		else
			return "var_" + std::to_string(id);
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
		case SSAOp::ASSIGN: return arg1.toString() + " = " + arg2.toString();
		case SSAOp::PLUS  : return arg1.toString() + " + " + arg2.toString();
		case SSAOp::MINUS : return arg1.toString() + " - " + arg2.toString();
		case SSAOp::MUL   : return arg1.toString() + " * " + arg2.toString();
		case SSAOp::DIV   : return arg1.toString() + " / " + arg2.toString();
		case SSAOp::MOD   : return arg1.toString() + " % " + arg2.toString();
		case SSAOp::ELEM  : return arg1.toString() + "[" + arg2.toString() + "]";
		case SSAOp::DEREF : return "*" + arg1.toString();
		case SSAOp::ADDR  : return "&" + arg1.toString();
		case SSAOp::PARAM : return "param " + arg1.toString();
		case SSAOp::CALL  : return "call " + arg1.toString() + ' ' + std::to_string(arg2.id);
		case SSAOp::LABEL : return arg1.toString();
		case SSAOp::RETURN: return "return " + arg1.toString();
		}
	}

	SSAOp op;
	Arg arg1;
	Arg arg2;
};

class GenSSAVisitor : public ASTVisitor
{
public:
	
	GenSSAVisitor();

	Arg getArg(AST* node);

	void visit(ImportNode *node) override;
	void visit(IfNode *node) override;
	void visit(ForNode *node) override;
	void visit(WhileNode *node) override;
	void visit(BracketNode *node) override;
	void visit(UnaryNode *node) override;
	void visit(NewExpressionNode *node) override;
	void visit(BinaryOperatorNode *node) override;
	void visit(StructDeclarationNode *node) override;
	void visit(FunctionDeclarationNode *node) override;
	void visit(VariableDeclarationNode *node) override;
	void visit(AddrNode *node) override;
	void visit(NullNode *node) override;
	void visit(DotNode *node) override;
	void visit(StatementNode *node) override;
	void visit(VariableNode *node) override;
	void visit(StringNode *node) override;
	void visit(NumberNode *node) override;
	void visit(CallNode *node) override;
	void visit(ReturnNode *node) override;
	void visit(UnsafeBlockNode *node) override;
	void visit(AsmArrayNode *node) override;
	void visit(VarInferTypeDeclarationNode *node) override;

	std::string toString();

private:

	std::vector<Command> commands;
	std::vector<int> code;

	void add(Command command);
	Arg newTemp();
	Arg newLabel(std::string label = "");

	Arg _arg;
};

#endif
