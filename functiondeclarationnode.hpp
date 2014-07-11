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

#include "codeobject.hpp"

using std::vector;
using std::pair;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(string name, vector< pair<string, TypeInfo> > params, TypeInfo return_type_info, AST *statements, FunctionTraits traits);

    ~FunctionDeclarationNode() override;
    
    AST* copyTree() const override;

    void build_scope() override;

    Symbol* getDefinedSymbol() const override;

    void define() override;
    void check() override;

    CodeObject& gen() override;

	vector<AST*> getChildren() const override;
    
private:

    string name;
    vector< pair<string, TypeInfo> > params;
    TypeInfo return_type_info;
    AST *statements;

    FunctionTraits traits;

    FunctionSymbol *definedSymbol;

	vector<VariableSymbol*> params_symbols;

	CodeObject code_obj;
};

#endif
