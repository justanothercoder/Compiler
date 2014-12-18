#ifndef _DISPOSEMEMORYVISITOR_HPP_
#define _DISPOSEMEMORYVISITOR_HPP_

#include "commandvisitor.hpp"

class SpaceAllocator;

class DisposeMemoryVisitor : public CommandVisitor
{
public:

    DisposeMemoryVisitor(SpaceAllocator& alloc);

    void visit(IfFalseCommand* command) override;
    void visit(GotoCommand* command) override;
    void visit(LabelCommand* command) override;
    void visit(ElemCommand* command) override;
    void visit(NewCommand* command) override;
    void visit(AssignCommand* command) override;
    void visit(ParamCommand* command) override;
    void visit(BinaryOpCommand* command) override;
    void visit(UnaryOpCommand* command) override;
    void visit(DotCommand* command) override;
    void visit(ReturnCommand* command) override;
    void visit(CallCommand* command) override;    

private:

    SpaceAllocator& alloc;
};

#endif
