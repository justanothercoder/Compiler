#ifndef _TEMPLATEHELPER_HPP_
#define _TEMPLATEHELPER_HPP_

#include "structsymbol.hpp"
#include "templatestructdeclarationnode.hpp"

class TemplateHelper
{
public:

    static const vector<Symbol*>& getNeededMembers(StructSymbol *sym);
    static void addNeededMember(StructSymbol *sym, Symbol *needed);

    static AST* getNode(TemplateStructSymbol *sym);
    static void setNode(TemplateStructSymbol *sym, AST *node);

    static StructSymbol* getSpec(TemplateStructSymbol *sym, const vector<ExprNode*>& symbols, const std::vector<DeclarationNode*>& inner);

private:

    static map< vector<ExprNode*>, StructSymbol*> specs;   

    static map< TemplateStructSymbol*, AST* > nodes;
    static map< StructSymbol*, vector<Symbol*> > needed_members;
    
};

#endif
