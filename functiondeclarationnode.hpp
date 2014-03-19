#ifndef _FUNCTIONDECLARATIONNODE_HPP_
#define _FUNCTIONDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "typehelper.hpp"

#include "variablesymbol.hpp"

#include "variablesymboldefine.hpp"
#include "functionsymboldefine.hpp"

#include "codegen.hpp"

using std::vector;
using std::pair;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, AST *statements, FunctionTraits traits);

    virtual ~FunctionDeclarationNode();
    
    virtual AST* copyTree() const;

    virtual void build_scope();    

    virtual Symbol* getDefinedSymbol() const;

    virtual void define(const TemplateInfo& template_info);
    virtual void check(const TemplateInfo& template_info);
    virtual void gen(const TemplateInfo& template_info);

	virtual vector<AST*> getChildren() const;
    
private:

    string name;
    vector< pair<string, TypeInfo> > params;
    TypeInfo return_type_info;
    AST *statements;

    FunctionTraits traits;

    FunctionSymbol *definedSymbol;
};

#endif
