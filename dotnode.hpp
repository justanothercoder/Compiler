#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"

class CodeObject;
class StructSymbol;
class VariableSymbol;

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, std::string member_name);   
	~DotNode() override;
    
    void check() override;
    CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

private:

    ExprNode *base;

	std::string member_name;

    const StructSymbol *base_type;
    VariableSymbol *member;
	CodeObject* code_obj;
};

#endif
