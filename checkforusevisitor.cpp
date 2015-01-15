#include "checkforusevisitor.hpp"

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

#include "temporaryarg.hpp"
#include "dotarg.hpp"

CheckForUseVisitor::CheckForUseVisitor() { }

bool CheckForUseVisitor::isUsed(Command* command) const { return used_commands.count(command); }
    
void CheckForUseVisitor::checkAndMark(Arg* arg)
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(arg) ) {
        used_commands.insert(temp -> command());
    }
    else if ( auto dot = dynamic_cast<DotArg*>(arg) ) {
        if ( auto temp = dynamic_cast<TemporaryArg*>(dot -> expr()) ) {
            used_commands.insert(temp -> command());
        }
    }
}

void CheckForUseVisitor::visit(IfFalseCommand* command) 
{
    used_commands.insert({command});
    checkAndMark(command -> expr());
}

void CheckForUseVisitor::visit(ElemCommand* command) 
{
    checkAndMark(command -> base());
    checkAndMark(command -> expr());
}

void CheckForUseVisitor::visit(AssignCommand* command) 
{
    used_commands.insert(command);
    checkAndMark(command -> lhs());
    checkAndMark(command -> rhs());
}

void CheckForUseVisitor::visit(AssignRefCommand* command)
{
    used_commands.insert(command);
    checkAndMark(command -> lhs());
    checkAndMark(command -> rhs());
}

void CheckForUseVisitor::visit(ParamCommand* command) 
{
    used_commands.insert(command);
    checkAndMark(command -> expr());
}

void CheckForUseVisitor::visit(BinaryOpCommand* command) 
{
    checkAndMark(command -> lhs());
    checkAndMark(command -> rhs());
}

void CheckForUseVisitor::visit(UnaryOpCommand* command) 
{
    checkAndMark(command -> expr());
}

void CheckForUseVisitor::visit(ReturnCommand* command) 
{
    used_commands.insert(command);
    checkAndMark(command -> expr());
}

void CheckForUseVisitor::visit(CallCommand* command)  { used_commands.insert(command); }
void CheckForUseVisitor::visit(GotoCommand* command)  { used_commands.insert(command); }
void CheckForUseVisitor::visit(LabelCommand* command) { used_commands.insert(command); }
void CheckForUseVisitor::visit(NewCommand* command)   { used_commands.insert(command); }
