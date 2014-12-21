#ifndef _COMMANDVISITOR_HPP_
#define _COMMANDVISITOR_HPP_

class IfFalseCommand;
class GotoCommand;
class LabelCommand;
class ElemCommand;
class NewCommand;
class AssignCommand;
class ParamCommand;
class BinaryOpCommand;
class UnaryOpCommand;
class DotCommand;
class ReturnCommand;
class CallCommand;
class AssignRefCommand;

class CommandVisitor
{
public:

    virtual void visit(IfFalseCommand* command) = 0;
    virtual void visit(GotoCommand* command) = 0;
    virtual void visit(LabelCommand* command) = 0;
    virtual void visit(ElemCommand* command) = 0;
    virtual void visit(NewCommand* command) = 0;
    virtual void visit(AssignCommand* command) = 0;
    virtual void visit(ParamCommand* command) = 0;
    virtual void visit(BinaryOpCommand* command) = 0;
    virtual void visit(UnaryOpCommand* command) = 0;
    virtual void visit(DotCommand* command) = 0;
    virtual void visit(ReturnCommand* command) = 0;
    virtual void visit(CallCommand* command) = 0;
    virtual void visit(AssignRefCommand* command) = 0;

};

#endif
