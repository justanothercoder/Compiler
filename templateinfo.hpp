#ifndef _TEMPLATEINFO_HPP_
#define _TEMPLATEINFO_HPP_

#include <boost/optional.hpp>

#include <vector>
#include <string>

#include "templateparam.hpp"

class TemplateStructSymbol;

using namespace std;

class ExprNode;

struct TemplateInfo
{
	TemplateInfo();
	TemplateInfo(TemplateStructSymbol *sym, std::vector<TemplateParam> expr);

	boost::optional<TemplateParam> getReplacement(string name) const;

	TemplateStructSymbol *sym;
	std::vector<TemplateParam> expr;
};

#endif
