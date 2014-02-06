#ifndef _FUNCTIONDECLARATIONNODE_HPP_
#define _FUNCTIONDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "typehelper.hpp"

#include "variablesymbol.hpp"

using std::vector;
using std::pair;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, const vector<AST*>& statements, FunctionTraits traits);

    virtual ~FunctionDeclarationNode();
    
    virtual void build_scope();    
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual Symbol* getDefinedSymbol() const;

    virtual void template_define(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual bool isTemplated() const;
    
private:

    string name;
    vector< pair<string, TypeInfo> > params;
    TypeInfo return_type_info;
    vector< AST* > statements;

    FunctionTraits traits;

    FunctionSymbol *definedSymbol;
};

#endif
