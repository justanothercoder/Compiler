#include "genssavisitor.hpp"
#include "numbernode.hpp"
#include "binaryoperatornode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "callnode.hpp"
#include "functiondeclarationnode.hpp"
#include "structdeclarationnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "addrnode.hpp"
#include "bracketnode.hpp"
#include "unarynode.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "dotnode.hpp"
#include "stringnode.hpp"
#include "builtins.hpp"
#include "optimizer.hpp"
#include "functionsymbol.hpp"

GenSSAVisitor::GenSSAVisitor() : _arg(IdType::NOID, -1)
{
    code.newBlock(*BuiltIns::global_scope);
}

Arg GenSSAVisitor::getArg(AST *node)
{
	node -> accept(*this);
	return _arg;
}

void GenSSAVisitor::visit(ImportNode *)
{
	
}

void GenSSAVisitor::visit(IfNode *node)
{
	auto false_label = code.newLabel();
	auto exit_label  = code.newLabel();	

	auto expr = getArg(node -> cond);

	GlobalHelper::addConst(0);
	auto zero = Arg(IdType::NUMBER, GlobalHelper::const_num_id[0], BuiltIns::int_type);

	auto temp = code.add(Command(SSAOp::EQUALS, expr, zero));

	code.add(Command(SSAOp::IFFALSE, temp, false_label));

	node -> stats_true -> accept(*this);	

	code.add(Command(SSAOp::GOTO, exit_label));
	code.add(Command(SSAOp::LABEL, false_label));

	node -> stats_false -> accept(*this);

	code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(ForNode *node)
{
	auto cycle_label = code.newLabel();
	auto exit_label  = code.newLabel();

	node -> init -> accept(*this);
	code.add(Command(SSAOp::LABEL, cycle_label));

	auto cond = getArg(node -> cond);

	GlobalHelper::addConst(0);
	auto zero = Arg(IdType::NUMBER, GlobalHelper::const_num_id[0], BuiltIns::int_type);

	auto temp = code.add(Command(SSAOp::EQUALS, cond, zero));

	code.add(Command(SSAOp::IFFALSE, temp, exit_label));

	node -> stats -> accept(*this);
	node -> step  -> accept(*this);

	code.add(Command(SSAOp::GOTO, cycle_label));
	code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(WhileNode *node)
{
	auto exit_label  = code.newLabel();
	auto cycle_label = code.newLabel();

	code.add(Command(SSAOp::LABEL, cycle_label));
	
	auto cond = getArg(node -> cond);
	
	GlobalHelper::addConst(0);
	auto zero = Arg(IdType::NUMBER, GlobalHelper::const_num_id[0], BuiltIns::int_type);

	auto temp = code.add(Command(SSAOp::EQUALS, cond, zero));

	code.add(Command(SSAOp::IFFALSE, temp, exit_label));

	node -> stats -> accept(*this);	

	code.add(Command(SSAOp::GOTO, cycle_label));
	code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(BracketNode *node)
{
	code.add(Command(SSAOp::PARAM, getArg(node -> expr)));
	code.add(Command(SSAOp::PARAM, getArg(node -> base)));
	
	_arg = code.add(Command(SSAOp::CALL, Arg(IdType::PROCEDURE, GlobalHelper::id_by_func[node -> call_info.callee]), Arg(IdType::NOID, 2)));
}

void GenSSAVisitor::visit(UnaryNode *node)
{
	code.add(Command(SSAOp::PARAM, getArg(node -> exp)));
	
	_arg = code.add(Command(SSAOp::CALL, Arg(IdType::PROCEDURE, GlobalHelper::id_by_func[node -> call_info.callee]), Arg(IdType::NOID, 1)));
}

void GenSSAVisitor::visit(NewExpressionNode *)
{

}

void GenSSAVisitor::visit(BinaryOperatorNode *node)
{
    auto lhs_type = node -> lhs -> getType();
    auto rhs_type = node -> rhs -> getType();    

    if ( lhs_type -> getUnqualifiedType() == BuiltIns::int_type 
      && rhs_type -> getUnqualifiedType() == BuiltIns::int_type )
    {
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
            default: throw std::logic_error("internal error");
        }

        _arg = code.add(Command(op, lhs, rhs));
    }
    else
    {
        code.add(Command(SSAOp::PARAM, getArg(node -> rhs)));
        code.add(Command(SSAOp::PARAM, getArg(node -> lhs)));
        
        _arg = code.add(Command(SSAOp::CALL, Arg(IdType::PROCEDURE, GlobalHelper::id_by_func[node -> call_info.callee]), 
                                             Arg(IdType::NOID, lhs_type -> getSize() + rhs_type -> getSize())
        ));
    }
}

void GenSSAVisitor::visit(StructDeclarationNode *node)
{
    code.newBlock(*node -> definedSymbol);
    for ( auto decl : node -> inner )
        decl -> accept(*this);
    code.popBlock();
}

void GenSSAVisitor::visit(FunctionDeclarationNode *node)
{
    code.newBlock(*node -> definedSymbol, node -> definedSymbol -> getScopedTypedName());
	code.add(Command(SSAOp::LABEL, code.newLabel(node -> definedSymbol -> getScopedTypedName())));

	node -> statements -> accept(*this);

    code.popBlock();
}

void GenSSAVisitor::visit(VariableDeclarationNode *)
{

}

void GenSSAVisitor::visit(AddrNode *node)
{
	_arg = code.add(Command(SSAOp::ADDR, getArg(node -> expr)));
}

void GenSSAVisitor::visit(NullNode *)
{
	GlobalHelper::addConst(0);

	_arg = Arg(IdType::NUMBER, GlobalHelper::const_num_id[0], BuiltIns::int_type);
}

void GenSSAVisitor::visit(DotNode *node)
{
    _arg = code.add(
            Command(SSAOp::DOT, 
                getArg(node -> base), 
                Arg(IdType::VARIABLE, GlobalHelper::id_by_var[node -> member], node -> member -> getType())
            )
    );
}

void GenSSAVisitor::visit(StatementNode *node)
{
	for ( auto stat : node -> statements )
		stat -> accept(*this);
}

void GenSSAVisitor::visit(VariableNode *node)
{
	_arg = Arg(IdType::VARIABLE, node -> getVarId(), node -> variable -> getType());
}

void GenSSAVisitor::visit(StringNode *node)
{
	_arg = Arg(IdType::STRING, node -> getStrId());
}

void GenSSAVisitor::visit(NumberNode *node)
{
	_arg = Arg(IdType::NUMBER, node -> getNumId(), BuiltIns::int_type);
}

void GenSSAVisitor::visit(CallNode *node)
{
	for ( auto param = node -> params.rbegin(); param != node -> params.rend(); ++param )
		code.add(Command(SSAOp::PARAM, getArg(*param)));

    int params_size = 0;
    for ( auto param : node -> params )
        params_size += param -> getType() -> getSize();    

	_arg = code.add(Command(SSAOp::CALL, 
			        Arg(IdType::PROCEDURE, GlobalHelper::id_by_func[node -> call_info.callee]), 
					Arg(IdType::NOID, params_size))
	);
}

void GenSSAVisitor::visit(ReturnNode *node)
{
	code.add(Command(SSAOp::RETURN, getArg(node -> expr)));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
	node -> block -> accept(*this);
}

void GenSSAVisitor::visit(AsmArrayNode *)
{

}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *) 
{

}
	
std::string GenSSAVisitor::getString()
{
	return code.toString();
}
	
const ThreeAddressCode& GenSSAVisitor::getCode() const
{
	return code;
}

void GenSSAVisitor::optimize()
{
    Optimizer optimizer(code);
    optimizer.optimize();
}
