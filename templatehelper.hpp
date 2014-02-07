#ifndef _TEMPLATEHELPER_HPP_
#define _TEMPLATEHELPER_HPP_

#include "structsymbol.hpp"
#include "templatedeclholder.hpp"

class TemplateHelper
{
public:

    static const vector<Symbol*>& getNeededMembers(StructSymbol *sym);
    static void addNeededMember(const StructSymbol *sym, Symbol *needed);

private:

    static map< StructSymbol*, vector<Symbol*> > needed_members;
    
};

#endif
