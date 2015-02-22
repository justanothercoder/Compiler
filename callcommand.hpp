#ifndef _CALLCOMMAND_HPP_
#define _CALLCOMMAND_HPP_

#include "command.hpp"

class FunctionalSymbol;

class CallCommand : public Command 
{
public:

    CallCommand(const FunctionalSymbol* function, int params_size);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    const FunctionalSymbol* function() const;

private:

    int params_size;
    const FunctionalSymbol* function_;

};

#endif
