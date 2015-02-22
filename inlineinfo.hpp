#ifndef _INLINEINFO_HPP_
#define _INLINEINFO_HPP_

#include <vector>
#include "ast.hpp"

class VarSymbol;

struct InlineInfo
{
    InlineInfo();
    InlineInfo(ASTNode function_body, std::vector<const VarSymbol*> locals);

    ASTNode function_body;
    std::vector<const VarSymbol*> locals;
};

#endif
