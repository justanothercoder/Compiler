#include "genssavisitor.hpp"
#include "numbernode.hpp"
#include "binaryoperatornode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "callnode.hpp"
#include "functiondeclarationnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"

GenSSAVisitor::GenSSAVisitor() : _arg(IdType::NOID, -1)
{

}

Arg GenSSAVisitor::newTemp()
{
	static int temp_num = 0;
	return Arg(IdType::TEMP, ++temp_num);
}
	
Arg GenSSAVisitor::newLabel(std::string label)
{
	static int label_num = 0;

	++label_num;

	if ( label == "" )
		GlobalHelper::label_name[label_num] = "label_" + std::to_string(label_num);
	else
		GlobalHelper::label_name[label_num] = label;

	return Arg(IdType::LABEL, label_num);
}
	
void GenSSAVisitor::add(Command command)
{
	commands.push_back(command);
	code.push_back(commands.size() - 1);
}

std::string GenSSAVisitor::toString()
{
	std::string res;
	
	for ( size_t i = 0; i < commands.size(); ++i )
		res += "command " + std::to_string(i) + ": " + commands[i].toString() + '\n';
	res += '\n';
	res += "code:\n";

	for ( auto i : code )
		res += std::to_string(i) + '\n'; 

//	for ( auto command : code )
//		res += commands[command].toString() + "\n";
	return res;
}
	
Arg GenSSAVisitor::getArg(AST* node)
{
	node -> accept(this);
	return _arg;
}

void GenSSAVisitor::visit(ImportNode *)
{
	
}

void GenSSAVisitor::visit(IfNode *)
{

}

void GenSSAVisitor::visit(ForNode *)
{

}

void GenSSAVisitor::visit(WhileNode *)
{

}

void GenSSAVisitor::visit(BracketNode *node)
{

}

void GenSSAVisitor::visit(UnaryNode *)
{

}

void GenSSAVisitor::visit(NewExpressionNode *)
{

}

void GenSSAVisitor::visit(BinaryOperatorNode *node)
{
/*
	auto rhs = getArg(node -> rhs);
	auto lhs = getArg(node -> lhs);

	SSAOp op;
	switch ( node -> op_type )
	{
		case BinaryOp::PLUS  : op = SSAOp::PLUS  ; break;
		case BinaryOp::MINUS : op = SSAOp::MINUS ; break;
		case BinaryOp::MUL   : op = SSAOp::MUL   ; break;
		case BinaryOp::DIV   : op = SSAOp::DIV   ; break;
		case BinaryOp::MOD   : op = SSAOp::MOD   ; break;
		case BinaryOp::ASSIGN: op = SSAOp::ASSIGN; break;
	}

	add(Command(op, lhs, rhs));
	_arg = Arg(IdType::COMMAND, commands.size() - 1);
*/
	
	add(Command(SSAOp::PARAM, getArg(node -> rhs)));
	add(Command(SSAOp::PARAM, getArg(node -> lhs)));
	
	add(Command(SSAOp::CALL, Arg(IdType::PROCEDURE, GlobalHelper::id_by_func[node -> call_info.callee]), Arg(IdType::NOID, 2)));
	_arg = Arg(IdType::COMMAND, commands.size() - 1);
}

void GenSSAVisitor::visit(StructDeclarationNode *)
{

}

void GenSSAVisitor::visit(FunctionDeclarationNode *node)
{
	add(Command(SSAOp::LABEL, newLabel(node -> definedSymbol -> getScopedTypedName())));
	node -> statements -> accept(this);	
}

void GenSSAVisitor::visit(VariableDeclarationNode *)
{

}

void GenSSAVisitor::visit(AddrNode *)
{

}

void GenSSAVisitor::visit(NullNode *)
{

}

void GenSSAVisitor::visit(DotNode *)
{

}

void GenSSAVisitor::visit(StatementNode *node)
{
	for ( auto stat : node -> statements )
		stat -> accept(this);
}

void GenSSAVisitor::visit(VariableNode *node)
{
	_arg = Arg(IdType::VARIABLE, node -> getVarId());
}

void GenSSAVisitor::visit(StringNode *)
{

}

void GenSSAVisitor::visit(NumberNode *node)
{
	_arg = Arg(IdType::NUMBER, node -> getNumId());
}

void GenSSAVisitor::visit(CallNode *node)
{
	for ( auto param = node -> params.rbegin(); param != node -> params.rend(); ++param )
		add(Command(SSAOp::PARAM, getArg(*param)));

//	add(Command(SSAOp::CALL, getArg(node -> caller), Arg(IdType::NOID, node -> params.size())));
	add(Command(SSAOp::CALL, Arg(IdType::PROCEDURE, GlobalHelper::id_by_func[node -> call_info.callee]), Arg(IdType::NOID, node -> params.size())));
	_arg = Arg(IdType::COMMAND, commands.size() - 1);
}

void GenSSAVisitor::visit(ReturnNode *node)
{
	add(Command(SSAOp::RETURN, getArg(node -> expr)));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
	node -> block -> accept(this);
}

void GenSSAVisitor::visit(AsmArrayNode *)
{

}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *) 
{

}
