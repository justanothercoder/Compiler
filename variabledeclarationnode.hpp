#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "variablesymbol.hpp"
#include "codeobject.hpp"
#include "typeinfo.hpp"
#include "typehelper.hpp"
#include "functionsymbol.hpp"
#include "functionhelper.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"
#include "callhelper.hpp"
#include "templateinfo.hpp"

#include "variablesymboldefine.hpp"

using std::vector;

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(string name, TypeInfo type_info, bool is_field = false, const vector<ExprNode*>& constructor_call_params = {});

    virtual ~VariableDeclarationNode();

    virtual AST* copyTree() const;
    
    void define() override;
    void check() override;
    
	CodeObject& gen() override;
    
    Symbol* getDefinedSymbol() const override;

	vector<AST*> getChildren() const override;

private:

    string name;
    TypeInfo type_info;

    bool is_field;

    VariableSymbol *definedSymbol;

    vector<ExprNode*> constructor_call_params;

	CallInfo call_info;

	CodeObject code_obj;
};

#endif
