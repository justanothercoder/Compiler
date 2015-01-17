#ifndef _TEMPLATEINFO_HPP_
#define _TEMPLATEINFO_HPP_

#include <vector>
#include <string>
#include <map>
#include <boost/optional.hpp>
#include "templateparam.hpp"

class ExprNode;
class TemplateSymbol;

struct TemplateInfo
{
    TemplateInfo();
    TemplateInfo(const TemplateSymbol *sym, std::vector<TemplateParam> template_params);

    boost::optional<TemplateParam> getReplacement(std::string name) const;
    bool isIn(std::string name) const;    

    const TemplateSymbol* sym;
    std::vector<TemplateParam> template_params;
};

using TemplateArgument = TemplateParam;
using TemplateMapping = std::map<std::string, TemplateArgument>;

#endif
