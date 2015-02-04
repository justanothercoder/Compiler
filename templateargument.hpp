#ifndef _TEMPLATEARGUMENT_HPP_
#define _TEMPLATEARGUMENT_HPP_

#include <boost/variant.hpp>
#include "typeinfo.hpp"

using TemplateArgument = boost::variant<TypeInfo, int>;
using TemplateArguments = std::vector<TemplateArgument>;
using TemplateParamsInfo = std::vector< std::pair<std::string, TypeInfo> >;

unsigned long long hashTemplateArguments(const TemplateArguments& template_params);
TemplateArgument getTemplateArgument(TemplateArgumentInfo info);
std::string toString(const TemplateArgumentsInfo& info);

#endif
