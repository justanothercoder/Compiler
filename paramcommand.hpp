#ifndef _PARAMCOMMAND_HPP_
#define _PARAMCOMMAND_HPP_

#include "command.hpp"
#include "conversioninfo.hpp"

class ParamCommand : public Command
{
public:

    ParamCommand(Arg* expr, ConversionInfo conversion_info);
    
    void gen(const Block& block, CodeObject& code_obj) const override;
    std::string toString() const override;
    bool isExpr() const override;

private:

    Arg* expr;
    ConversionInfo conversion_info;
};

#endif
