#ifndef _TEMPLATEINFO_HPP_
#define _TEMPLATEINFO_HPP_

#include <boost/optional.hpp>

#include <vector>
#include <string>

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

#endif
