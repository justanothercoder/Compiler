#ifndef _TEMPLATEPARAM_HPP_
#define _TEMPLATEPARAM_HPP_

#include <boost/variant.hpp>
#include "typeinfo.hpp"

using TemplateParam = boost::variant<TypeInfo, int>;

#endif
