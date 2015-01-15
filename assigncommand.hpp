#ifndef _ASSIGNCOMMAND_HPP_
#define _ASSIGNCOMMAND_HPP_

#include <memory>
#include "command.hpp"

class AssignCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    AssignCommand(Argument lhs, Argument rhs, bool is_char);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* lhs();
    Arg* rhs();
    bool isChar() const;

public:

    Argument lhs_;
    Argument rhs_;
    bool is_char;
};

#endif
