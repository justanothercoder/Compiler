#include "templatehelper.hpp"
#include "structdeclarationnode.hpp"

map< StructSymbol*, vector<Symbol*> > TemplateHelper::needed_members;

map< vector<ExprNode*>, StructSymbol*> TemplateHelper::specs;

const vector<Symbol*>& TemplateHelper::getNeededMembers(StructSymbol* sym)
{
    return needed_members[sym];
}

void TemplateHelper::addNeededMember(StructSymbol *sym, Symbol *needed)
{
    needed_members[sym].push_back(needed);
}

StructSymbol* TemplateHelper::getSpec(TemplateStructSymbol *sym, const vector<ExprNode*>& symbols, TemplateDeclHolder *holder)
{
    auto it = specs.find(symbols);
    if ( it != std::end(specs) )
	return it->second;

    auto hash_func = [](const vector<ExprNode*>& vec)
	{
	    long long P = 31, pow = 1, ans = 0;
	    
	    for ( int i = 0; i < static_cast<int>(vec.size()); ++i )
	    {
		ans += ((long long)vec[i]) * pow;
		pow *= P;		
	    }
	    
	    return ans;
	};

    StructDeclarationNode *decl = new StructDeclarationNode(sym->getName() + std::to_string(hash_func(symbols)), decl->getInner());
}

