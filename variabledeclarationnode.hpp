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

using std::vector;

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(string name, TypeInfo type_info, bool is_field = false, const vector<ExprNode*>& constructor_call_params = {});

    virtual ~VariableDeclarationNode();
    
    virtual void build_scope();    
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual Symbol* getDefinedSymbol() const;
    
private:

    string name;
    TypeInfo type_info;

    bool is_field;

    VariableSymbol *definedSymbol;

    vector<ExprNode*> constructor_call_params;

    FunctionSymbol *resolved_constructor;
};

#endif
