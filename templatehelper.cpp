#include "templatehelper.hpp"

map< StructSymbol*, vector<Symbol*> > TemplateHelper::needed_members;

map< TemplateStructSymbol*, AST*> TemplateHelper::nodes;

const vector<Symbol*>& TemplateHelper::getNeededMembers(StructSymbol* sym)
{
    return needed_members[sym];
}

void TemplateHelper::addNeededMember(StructSymbol *sym, Symbol *needed)
{
    needed_members[sym].push_back(needed);
}

AST* TemplateHelper::getNode(TemplateStructSymbol *sym)
{
    auto it = nodes.find(sym);
    if ( it == std::end(nodes) )
	return nullptr;

    return it->second;
}

void TemplateHelper::setNode(TemplateStructSymbol *sym, AST *node)
{
    nodes[sym] = node;
}

StructSymbol* TemplateHelper::getSpec(TemplateStructSymbol *sym, const vector<ExprNode*>& symbols, const std::vector<DeclarationNode*>& inner)
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

    StructDeclarationNode *decl = new StructDeclarationNode(sym->getName() + std::to_string(hash_func(symbols)), inner);
}

