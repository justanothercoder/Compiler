#ifndef _TEMPLATEINFO_HPP_
#define _TEMPLATEINFO_HPP_

#include <vector>
#include <string>

#include "templatestructsymbol.hpp"

using namespace std;

class ExprNode;

class TemplateInfo
{
public:
	TemplateInfo(TemplateStructSymbol *sym, std::vector<ExprNode*> expr);

	ExprNode* getReplacement(string name);

private:

	TemplateStructSymbol *sym;
	std::vector<ExprNode*> expr;
};

#endif
