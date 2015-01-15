#ifndef _ASSIGNREFCOMMAND_HPP_
#define _ASSIGNREFCOMMAND_HPP_

#include <memory>
#include "command.hpp"

class AssignRefCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    AssignRefCommand(Argument lhs, Argument rhs);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* lhs();
    Arg* rhs();

private:

    Argument lhs_;
    Argument rhs_;
};

#endif
