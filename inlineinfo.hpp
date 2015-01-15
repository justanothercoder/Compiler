#ifndef _INLINEINFO_HPP_
#define _INLINEINFO_HPP_

#include <vector>
#include "ast.hpp"

class VariableSymbol;

struct InlineInfo
{
    InlineInfo();
    InlineInfo(ASTNode function_body, std::vector< std::shared_ptr<VariableSymbol> > locals);

    ASTNode function_body;
    std::vector< std::shared_ptr<VariableSymbol> > locals;
};

#endif
