#include "inlineinfo.hpp"

InlineInfo::InlineInfo() : InlineInfo(nullptr, { }) { }

InlineInfo::InlineInfo(ASTNode function_body, std::vector< std::shared_ptr<VariableSymbol> > locals) : function_body(std::move(function_body))
                                                                                                     , locals       (std::move(locals))
{

}
