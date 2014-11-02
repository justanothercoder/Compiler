#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "templatesymbol.hpp"
#include "typeinfo.hpp"
#include "structsymbol.hpp"
#include "templateparam.hpp"

class AST;
class StructSymbol;
class StructDeclarationNode;

class TemplateStructSymbol : public StructSymbol, public TemplateSymbol
{
public:

    TemplateStructSymbol(std::string name, Scope *enclosing_scope, std::vector< std::pair<std::string, TypeInfo> > template_symbols, AST *holder);

    SymbolType getSymbolType() const override;

    bool isIn(std::string name) const;

public:

    mutable std::map< long long, StructDeclarationNode*> specs;

    std::vector< std::pair<std::string, TypeInfo> > template_symbols;
    AST *holder;
};

#endif
