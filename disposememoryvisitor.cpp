#include "disposememoryvisitor.hpp"
#include "spaceallocator.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "variablearg.hpp"
#include "temporaryarg.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "type.hpp"

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

void DisposeMemoryVisitor::visit(IfFalseCommand* )   { }
void DisposeMemoryVisitor::visit(GotoCommand* )      { }
void DisposeMemoryVisitor::visit(LabelCommand* )     { }
void DisposeMemoryVisitor::visit(AssignCommand* )    { }
void DisposeMemoryVisitor::visit(ParamCommand* )     { }
void DisposeMemoryVisitor::visit(ReturnCommand* )    { }
void DisposeMemoryVisitor::visit(AssignRefCommand* ) { }
