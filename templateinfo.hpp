#ifndef _TEMPLATEINFO_HPP_
#define _TEMPLATEINFO_HPP_

#include <vector>
#include <string>

class TemplateStructSymbol;

#include "optional.hpp"
#include "templateparam.hpp"

using namespace std;

class ExprNode;

struct TemplateInfo
{
	TemplateInfo();
	TemplateInfo(TemplateStructSymbol *sym, std::vector<TemplateParam> expr);

	optional<TemplateParam> getReplacement(string name) const;

	TemplateStructSymbol *sym;
	std::vector<TemplateParam> expr;
};

#endif
