#ifndef _ELEMCOMMAND_HPP_
#define _ELEMCOMMAND_HPP_

#include <memory>
#include "command.hpp"

class ElemCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    ElemCommand(Argument base, Argument expr, bool is_string = false);

    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* base();
    Arg* expr();
    bool isStringElem() const;

private:

    Argument base_;
    Argument expr_;

    bool is_string;
};

#endif
