#include "command.hpp"
	
Command::Command(SSAOp op, Arg arg1, Arg arg2) : op(op), arg1(arg1), arg2(arg2), type(nullptr)
{

} 

Command::Command(Type *type, SSAOp op, Arg arg1, Arg arg2) : op(op), arg1(arg1), arg2(arg2), type(type)
{

} 	
	
std::string Command::toString()
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
    
bool Command::isExpr() const
{
    switch ( op )
    {
        case SSAOp::GOTO  : case SSAOp::IFFALSE: case SSAOp::IF   :
        case SSAOp::RETURN: case SSAOp::LABEL  : case SSAOp::PARAM:
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
