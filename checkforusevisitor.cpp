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
#include "dotcommand.hpp"
#include "returncommand.hpp"
#include "callcommand.hpp"

#include "temporaryarg.hpp"

CheckForUseVisitor::CheckForUseVisitor()
{
    
}

bool CheckForUseVisitor::isUsed(Command* command) const
{
    return used_commands.count(command);
}

void CheckForUseVisitor::visit(IfFalseCommand* command) 
{
    used_commands.insert({command});

    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(ElemCommand* command) 
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> base) )
        used_commands.insert(temp -> command);

    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(AssignCommand* command) 
{
    used_commands.insert(command);

    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> lhs) )
        used_commands.insert(temp -> command);
    
    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> rhs) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(ParamCommand* command) 
{
    used_commands.insert(command);

    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(BinaryOpCommand* command) 
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> lhs) )
        used_commands.insert(temp -> command);
    
    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> rhs) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(UnaryOpCommand* command) 
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(DotCommand* command) 
{
    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(ReturnCommand* command) 
{
    used_commands.insert(command);

    if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) )
        used_commands.insert(temp -> command);
}

void CheckForUseVisitor::visit(CallCommand* command) 
{
    used_commands.insert(command);
}

void CheckForUseVisitor::visit(GotoCommand* command) 
{
    used_commands.insert(command);
}

void CheckForUseVisitor::visit(LabelCommand* command) 
{
    used_commands.insert(command);
}

void CheckForUseVisitor::visit(NewCommand* command) 
{
    used_commands.insert(command);
}
