#include "disposememoryvisitor.hpp"
#include "spaceallocator.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "variablearg.hpp"
#include "temporaryarg.hpp"
#include "globalconfig.hpp"
#include "type.hpp"

#include "dotcommand.hpp"
#include "newcommand.hpp"
#include "elemcommand.hpp"
#include "callcommand.hpp"
#include "unaryopcommand.hpp"
#include "binaryopcommand.hpp"

DisposeMemoryVisitor::DisposeMemoryVisitor(SpaceAllocator& alloc) : alloc(alloc) { }

void DisposeMemoryVisitor::visit(NewCommand* command)      { alloc.remember(command, command -> type() -> sizeOf()); }
void DisposeMemoryVisitor::visit(ElemCommand* command)     { alloc.remember(command, GlobalConfig::int_size); }
void DisposeMemoryVisitor::visit(BinaryOpCommand* command) { alloc.remember(command, GlobalConfig::int_size); }
void DisposeMemoryVisitor::visit(UnaryOpCommand* command)  { alloc.remember(command, GlobalConfig::int_size); }
void DisposeMemoryVisitor::visit(CallCommand* command)     { alloc.remember(command, command -> function -> type().returnType().sizeOf()); }

void DisposeMemoryVisitor::visit(DotCommand* command) 
{
    int arg_addr;
    if ( dynamic_cast<VariableArg*>(command -> expr) )
    {
        auto base_type = command -> expr -> type();
        if ( base_type -> isReference() ) 
        {
            alloc.remember(command, GlobalConfig::int_size);
            return;
        }

        auto base_var = static_cast<VariableArg*>(command -> expr) -> var;
        if ( base_var -> isField() )
        {
            alloc.remember(command, GlobalConfig::int_size);
            return;
        }

        arg_addr = alloc.addressOf(base_var);
    }
    else if ( dynamic_cast<TemporaryArg*>(command -> expr) )
        arg_addr = alloc.addressOf(static_cast<TemporaryArg*>(command -> expr) -> command);
    else
        arg_addr = 0;

    alloc.rememberAt(command, arg_addr - command -> offset);
}

void DisposeMemoryVisitor::visit(IfFalseCommand* ) { }
void DisposeMemoryVisitor::visit(GotoCommand* )    { }
void DisposeMemoryVisitor::visit(LabelCommand* )   { }
void DisposeMemoryVisitor::visit(AssignCommand* )  { }
void DisposeMemoryVisitor::visit(ParamCommand* )   { }
void DisposeMemoryVisitor::visit(ReturnCommand* )  { }
