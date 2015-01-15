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
#include "returncommand.hpp"
#include "callcommand.hpp"
#include "assignrefcommand.hpp"

SubstituteArgVisitor::SubstituteArgVisitor(std::function<Argument(const Argument&)> substitutor) : substitutor(substitutor) { }

void SubstituteArgVisitor::visit(IfFalseCommand* command) 
{
    command -> expr_ = substitutor(command -> expr_);
}

void SubstituteArgVisitor::visit(ElemCommand* command) 
{
    command -> base_ = substitutor(command -> base_);
    command -> expr_ = substitutor(command -> expr_);
}

void SubstituteArgVisitor::visit(AssignCommand* command) 
{
    command -> lhs_ = substitutor(command -> lhs_);
    command -> rhs_ = substitutor(command -> rhs_);
}
    
void SubstituteArgVisitor::visit(AssignRefCommand* command) 
{
    command -> lhs_ = substitutor(command -> lhs_);
    command -> rhs_ = substitutor(command -> rhs_);
}

void SubstituteArgVisitor::visit(ParamCommand* command) 
{
    command -> expr_ = substitutor(command -> expr_);
}

void SubstituteArgVisitor::visit(BinaryOpCommand* command) 
{
    command -> lhs_ = substitutor(command -> lhs_);
    command -> rhs_ = substitutor(command -> rhs_);
}

void SubstituteArgVisitor::visit(UnaryOpCommand* command) 
{
    command -> expr_ = substitutor(command -> expr_);
}

void SubstituteArgVisitor::visit(ReturnCommand* command) 
{
    command -> expr_ = substitutor(command -> expr_);
}

void SubstituteArgVisitor::visit(CallCommand*  ) { }
void SubstituteArgVisitor::visit(GotoCommand*  ) { }
void SubstituteArgVisitor::visit(LabelCommand* ) { }
void SubstituteArgVisitor::visit(NewCommand*   ) { }

