#include "templatehelper.hpp"
#include "structdeclarationnode.hpp"

map< StructSymbol*, vector<Symbol*> > TemplateHelper::needed_members;

const vector<Symbol*>& TemplateHelper::getNeededMembers(StructSymbol* sym)
{
    return needed_members[sym];
}

void TemplateHelper::addNeededMember(const StructSymbol *sym, Symbol *needed)
{
    needed_members[const_cast<StructSymbol*>(sym)].push_back(needed);
}
