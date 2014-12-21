#ifndef _CHECKFORUSEVISITOR_HPP_
#define _CHECKFORUSEVISITOR_HPP_

#include <set>
#include "commandvisitor.hpp"

class Command;

class CheckForUseVisitor : public CommandVisitor
{
public:

    CheckForUseVisitor();

    bool isUsed(Command* command) const;

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
    void visit(AssignRefCommand* command) override;    

private:

    std::set<Command*> used_commands;
};

#endif
