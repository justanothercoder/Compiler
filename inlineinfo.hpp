#ifndef _INLINEINFO_HPP_
#define _INLINEINFO_HPP_

#include <vector>

class AST;
class VariableSymbol;

struct InlineInfo
{
    InlineInfo();
    InlineInfo(AST* function_body, std::vector<VariableSymbol*> locals);

    AST* function_body;
    std::vector<VariableSymbol*> locals;
};

#endif
