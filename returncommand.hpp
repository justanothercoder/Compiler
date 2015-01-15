#ifndef _RETURNCOMMAND_HPP_
#define _RETURNCOMMAND_HPP_

#include "command.hpp"

class ReturnCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    ReturnCommand(Argument expr, bool is_return_ref);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* expr();
    bool isReturnRef() const;

private:

    Argument expr_;
    bool is_return_ref;
};

#endif
