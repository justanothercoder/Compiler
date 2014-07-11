#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "codeobject.hpp"
#include "functionsymboldefine.hpp"

using std::vector;

class StructSymbol;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(string name, const vector<AST*>& inner);

    ~StructDeclarationNode() override;

    AST* copyTree() const override;
    
    void build_scope();    

    Symbol* getDefinedSymbol() const override;

	void define() override;
    void check() override;
    
	CodeObject& gen() override;

	vector<AST*> getChildren() const override;

protected:

    string name;
    vector<AST*> inner;

    StructSymbol *definedSymbol;
	CodeObject code_obj;
	CodeObject default_copy_constr_code;
	CodeObject default_constr_code;
};

#endif
