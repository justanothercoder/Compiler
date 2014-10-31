#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"

class NumberNode;
class VariableSymbol;

class VariableNode : public ExprNode
{

	friend class TemplateStructSymbol;
	friend class Scope;

	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:

    VariableNode(std::string name);

    AST* copyTree() const override;

    bool isTemplateParam() const;
    
    const Type* getType() const override;
	bool isLeftValue() const override;

	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

private:

	std::string name;
    
    VariableSymbol *variable;
	NumberNode *template_num;
};

#endif
