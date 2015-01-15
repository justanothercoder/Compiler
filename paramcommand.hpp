#ifndef _PARAMCOMMAND_HPP_
#define _PARAMCOMMAND_HPP_

#include "command.hpp"
#include "conversioninfo.hpp"

class ParamCommand : public Command
{
    friend class SubstituteArgVisitor;
public:

    ParamCommand(Argument expr, ConversionInfo conversion_info);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

    const Type* type() const override;
    void accept(CommandVisitor* visitor) override;

    Arg* expr();
    const ConversionInfo& conversionInfo() const;

private:

    Argument expr_;
    ConversionInfo conversion_info;
};

#endif
