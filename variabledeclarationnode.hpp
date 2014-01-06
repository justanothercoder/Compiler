#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "variablesymbol.hpp"
#include "codegen.hpp"
#include "typeinfo.hpp"
#include "typehelper.hpp"

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(string name, TypeInfo type_info, bool is_field = false);

    virtual void build_scope();    
    virtual void define();
    virtual void check();
    virtual void gen();

private:

    string name;
    TypeInfo type_info;

    bool is_field;
    
};

#endif
