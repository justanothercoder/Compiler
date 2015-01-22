#ifndef _TEMPLATEPARAM_HPP_
#define _TEMPLATEPARAM_HPP_

#include <boost/variant.hpp>
#include "typeinfo.hpp"

using TemplateParam = boost::variant<TypeInfo, int>;
using TemplateParamsList = std::vector< std::pair<std::string, TypeInfo> >;

unsigned long long hashTemplateParams(std::vector<TemplateParam> template_params);
    
#endif
