#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "variablesymbol.hpp"
#include "codegen.hpp"
#include "typeinfo.hpp"
#include "typehelper.hpp"
#include "functionsymbol.hpp"
#include "functionhelper.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"

#include "variablesymboldefine.hpp"

using std::vector;

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(string name, TypeInfo type_info, bool is_field = false, const vector<ExprNode*>& constructor_call_params = {});

    virtual ~VariableDeclarationNode();

    virtual AST* copyTree() const;
    
    virtual void build_scope();    

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    
    virtual Symbol* getDefinedSymbol() const;

	virtual vector<AST*> getChildren() const;
    
private:

    string name;
    TypeInfo type_info;

    bool is_field;

    VariableSymbol *definedSymbol;

    vector<ExprNode*> constructor_call_params;

    FunctionSymbol *resolved_constructor;
};

#endif
