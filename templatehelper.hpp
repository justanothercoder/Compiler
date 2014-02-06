#ifndef _TEMPLATEHELPER_HPP_
#define _TEMPLATEHELPER_HPP_

#include "structsymbol.hpp"
#include "templatedeclholder.hpp"

class TemplateHelper
{
public:

    static const vector<Symbol*>& getNeededMembers(StructSymbol *sym);
    static void addNeededMember(StructSymbol *sym, Symbol *needed);

    static StructSymbol* getSpec(TemplateStructSymbol *sym, const vector<ExprNode*>& symbols, TemplateDeclHolder *holder);

private:

    static map< vector<ExprNode*>, StructSymbol*> specs;   
    static map< StructSymbol*, vector<Symbol*> > needed_members;
    
};

#endif
