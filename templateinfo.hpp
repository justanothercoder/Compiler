#ifndef _TEMPLATEINFO_HPP_
#define _TEMPLATEINFO_HPP_

#include <vector>
#include <string>
#include <map>
#include <boost/optional.hpp>
#include "templateargument.hpp"

class ExprNode;
class Symbol;
class TemplateSymbol;

struct TemplateInfo
{
    TemplateInfo();
    TemplateInfo(const TemplateSymbol* sym, TemplateArguments template_arguments);

    boost::optional<TemplateArgument> getReplacement(std::string name) const;
    bool isIn(std::string name) const;    
    std::string getInstName() const;

    const TemplateSymbol* sym;
    TemplateArguments template_arguments;
};

#endif
