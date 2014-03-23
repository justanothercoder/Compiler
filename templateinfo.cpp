#include "templateinfo.hpp"

TemplateInfo::TemplateInfo() : TemplateInfo(nullptr, { }) { }
TemplateInfo::TemplateInfo(TemplateStructSymbol *sym, std::vector<ExprNode*> expr) : sym(sym), expr(expr) { }

ExprNode* TemplateInfo::getReplacement(string name) const
{
	for ( size_t i = 0; i < sym->template_symbols.size(); ++i )
	{
		if ( sym->template_symbols[i].first == name )
			return expr[i];
	}
	return nullptr;
}