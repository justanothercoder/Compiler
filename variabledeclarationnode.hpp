#ifndef _VARIABLEDECLARATIONNODE_HPP_
#define _VARIABLEDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "typeinfo.hpp"

class VariableSymbol;

class VariableDeclarationNode : public DeclarationNode
{
public:

    VariableDeclarationNode(std::string name, TypeInfo type_info, bool is_field = false, std::vector<ExprNode*> constructor_call_params = {});

    AST* copyTree() const override;
    
	void build_scope() override;
    
    Symbol* getDefinedSymbol() const override;

	std::vector<AST*> getChildren() const override;
	
	std::string toString() const override;

	void accept(ASTVisitor& visitor) override;

	std::string name;
    TypeInfo type_info;

    bool is_field;

    VariableSymbol *definedSymbol;

	std::vector<ExprNode*> constructor_call_params;

	CallInfo call_info;
};

#endif
