#include "substituteargvisitor.hpp"

#include "iffalsecommand.hpp"
#include "gotocommand.hpp"
#include "labelcommand.hpp"
#include "elemcommand.hpp"
#include "newcommand.hpp"
#include "assigncommand.hpp"
#include "paramcommand.hpp"
#include "binaryopcommand.hpp"
#include "unaryopcommand.hpp"
#include "dotcommand.hpp"
#include "returncommand.hpp"
#include "callcommand.hpp"
#include "assignrefcommand.hpp"

SubstituteArgVisitor::SubstituteArgVisitor(std::function<Arg*(Arg*)> substitutor) : substitutor(substitutor)
{
    
}

void SubstituteArgVisitor::visit(IfFalseCommand* command) 
{
    command -> expr = substitutor(command -> expr);
}

void SubstituteArgVisitor::visit(ElemCommand* command) 
{
    command -> base = substitutor(command -> base);
    command -> expr = substitutor(command -> expr);
}

void SubstituteArgVisitor::visit(AssignCommand* command) 
{
    command -> lhs = substitutor(command -> lhs);
    command -> rhs = substitutor(command -> rhs);
}
    
void SubstituteArgVisitor::visit(AssignRefCommand* command) 
{
    command -> lhs = substitutor(command -> lhs);
    command -> rhs = substitutor(command -> rhs);
}

void SubstituteArgVisitor::visit(ParamCommand* command) 
{
    command -> expr = substitutor(command -> expr);
}

void SubstituteArgVisitor::visit(BinaryOpCommand* command) 
{
    command -> lhs = substitutor(command -> lhs);
    command -> rhs = substitutor(command -> rhs);
}

void SubstituteArgVisitor::visit(UnaryOpCommand* command) 
{
    command -> expr = substitutor(command -> expr);
}

void SubstituteArgVisitor::visit(DotCommand* command) 
{
    command -> expr = substitutor(command -> expr);
}

void SubstituteArgVisitor::visit(ReturnCommand* command) 
{
    command -> expr = substitutor(command -> expr);
}

void SubstituteArgVisitor::visit(CallCommand* ) { }
void SubstituteArgVisitor::visit(GotoCommand* ) { }
void SubstituteArgVisitor::visit(LabelCommand* ) { }
void SubstituteArgVisitor::visit(NewCommand* ) { }

