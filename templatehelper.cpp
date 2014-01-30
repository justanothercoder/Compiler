#include "templatehelper.hpp"

map< StructSymbol*, vector<Symbol*> > TemplateHelper::needed_members;

const vector<Symbol*>& TemplateHelper::getNeededMembers(StructSymbol* sym)
{
    return needed_members[sym];
}

void TemplateHelper::addNeededMember(StructSymbol *sym, Symbol *needed)
{
    needed_members[sym].push_back(needed);
}
