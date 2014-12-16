#ifndef _CALLCOMMAND_HPP_
#define _CALLCOMMAND_HPP_

#include "command.hpp"

class FunctionSymbol;

class CallCommand : public Command 
{
public:

    CallCommand(const FunctionSymbol* function, int params_size);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    void setInlineCall();

    const FunctionSymbol* function;

private:

    int params_size;

    bool is_inline_call;
};

#endif
