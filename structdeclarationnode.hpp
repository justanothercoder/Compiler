#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "codeobject.hpp"
#include "functionsymboldefine.hpp"

class StructSymbol;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(std::string name, std::vector<AST*> inner);

    ~StructDeclarationNode() override;

    AST* copyTree() const override;
    
    void build_scope();    

    Symbol* getDefinedSymbol() const override;

	void define() override;
    void check() override;
    
	CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;

	std::string toString() const override;

protected:

	std::string name;
	std::vector<AST*> inner;

    StructSymbol *definedSymbol;
	CodeObject code_obj;
	CodeObject default_copy_constr_code;
	CodeObject default_constr_code;
};

#endif
