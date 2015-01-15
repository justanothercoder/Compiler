#ifndef _SUBSTITUTEARGVISITOR_HPP_
#define _SUBSTITUTEARGVISITOR_HPP_

#include <functional>
#include "commandvisitor.hpp"
#include "arg.hpp"

class SubstituteArgVisitor : public CommandVisitor
{
public:

    SubstituteArgVisitor(std::function<Argument(const Argument&)> substitutor);

    void visit(IfFalseCommand* command) override;
    void visit(GotoCommand* command) override;
    void visit(LabelCommand* command) override;
    void visit(ElemCommand* command) override;
    void visit(NewCommand* command) override;
    void visit(AssignCommand* command) override;
    void visit(ParamCommand* command) override;
    void visit(BinaryOpCommand* command) override;
    void visit(UnaryOpCommand* command) override;
    void visit(ReturnCommand* command) override;
    void visit(CallCommand* command) override;
    void visit(AssignRefCommand* command) override;

    std::function<Argument(const Argument&)> substitutor;
};

#endif
