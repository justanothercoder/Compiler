#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>
#include <boost/optional.hpp>

#include "declarationnode.hpp"
#include "codeobject.hpp"
#include "functionsymboldefine.hpp"
#include "templateinfo.hpp"

class StructSymbol;

class StructDeclarationNode : public DeclarationNode
{

public:

    StructDeclarationNode(std::string name, std::vector<AST*> inner, boost::optional<TemplateInfo> template_info = boost::none);

    AST* copyTree() const override;
    
    void build_scope();    

    Symbol* getDefinedSymbol() const override;

	CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

	std::string name;
	std::vector<AST*> inner;

	boost::optional<TemplateInfo> template_info;

    StructSymbol *definedSymbol;
	CodeObject code_obj;
	CodeObject default_copy_constr_code;
	CodeObject default_constr_code;
};

#endif
