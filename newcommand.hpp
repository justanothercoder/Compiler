#ifndef _NEWCOMMAND_HPP_
#define _NEWCOMMAND_HPP_

#include "command.hpp"

class Type;

class NewCommand : public Command
{
public:

    NewCommand(const Type* _type);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;

private:

    const Type* _type;
};

#endif
