#include "inlineinfo.hpp"

InlineInfo::InlineInfo() : InlineInfo(nullptr, { })
{

}

InlineInfo::InlineInfo(AST* function_body, std::vector<VariableSymbol*> locals) : function_body(function_body)
                                                                                , locals       (locals)
{

}
