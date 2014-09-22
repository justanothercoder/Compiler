#ifndef _FUNCTIONDECLARATIONNODE_HPP_
#define _FUNCTIONDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "codeobject.hpp"
#include "functiontraits.hpp"
#include "typeinfo.hpp"

class FunctionSymbol;
class VariableSymbol;

using std::pair;

class FunctionDeclarationNode : public DeclarationNode
{
public:

    FunctionDeclarationNode(std::string name
			              , std::vector< pair<std::string, TypeInfo> > params
						  , TypeInfo return_type_info
						  , AST *statements
						  , FunctionTraits traits
						  , bool is_unsafe = false);

    ~FunctionDeclarationNode() override;
    
    AST* copyTree() const override;

    void build_scope() override;

    Symbol* getDefinedSymbol() const override;

    void define() override;
    void check() override;

    CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;

	std::string toString() const override;
    
private:

	std::string name;
	std::vector< pair<std::string, TypeInfo> > params;
    TypeInfo return_type_info;
    AST *statements;

    FunctionTraits traits;

    FunctionSymbol *definedSymbol;

	std::vector<VariableSymbol*> params_symbols;

	CodeObject code_obj;

	bool is_unsafe;
};

#endif
