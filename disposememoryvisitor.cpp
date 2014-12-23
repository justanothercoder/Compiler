#include "disposememoryvisitor.hpp"
#include "spaceallocator.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "variablearg.hpp"
#include "temporaryarg.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "type.hpp"

#include "dotcommand.hpp"
#include "newcommand.hpp"
#include "elemcommand.hpp"
#include "callcommand.hpp"
#include "unaryopcommand.hpp"
#include "binaryopcommand.hpp"

DisposeMemoryVisitor::DisposeMemoryVisitor(SpaceAllocator& alloc) : alloc(alloc) { }

void DisposeMemoryVisitor::visit(NewCommand* command)      { alloc.remember(command, command -> type() -> sizeOf()); }
void DisposeMemoryVisitor::visit(ElemCommand* command)     { alloc.remember(command, Comp::config().int_size); }
void DisposeMemoryVisitor::visit(BinaryOpCommand* command) { alloc.remember(command, Comp::config().int_size); }
void DisposeMemoryVisitor::visit(UnaryOpCommand* command)  { alloc.remember(command, Comp::config().int_size); }
void DisposeMemoryVisitor::visit(CallCommand* command)     { alloc.remember(command, command -> function -> type().returnType().sizeOf()); }

void DisposeMemoryVisitor::visit(DotCommand* command) 
{
    if ( auto var = dynamic_cast<VariableArg*>(command -> expr) ) {
        if ( command -> expr -> type() -> isReference() ) {
            alloc.remember(command, Comp::config().int_size);
        }
        else {
            alloc.rememberAt(command, alloc.addressOf(var -> var) - command -> offset);
        }
    }
    else if ( auto temp = dynamic_cast<TemporaryArg*>(command -> expr) ) {
        alloc.rememberAt(command, alloc.addressOf(temp -> command) - command -> offset);
    }        
    else {
        throw "";
    }
}

void DisposeMemoryVisitor::visit(IfFalseCommand* )   { }
void DisposeMemoryVisitor::visit(GotoCommand* )      { }
void DisposeMemoryVisitor::visit(LabelCommand* )     { }
void DisposeMemoryVisitor::visit(AssignCommand* )    { }
void DisposeMemoryVisitor::visit(ParamCommand* )     { }
void DisposeMemoryVisitor::visit(ReturnCommand* )    { }
void DisposeMemoryVisitor::visit(AssignRefCommand* ) { }
