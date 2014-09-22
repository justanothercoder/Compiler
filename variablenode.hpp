#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

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

    void check() override;
    CodeObject& gen() override;

    AST* copyTree() const override;

    bool isTemplateParam() const;
    
    const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;

	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

	int getVarId() const;

private:

	std::string name;
    
    VariableSymbol *variable;

	CodeObject code_obj;
	
	NumberNode *template_num;
};

#endif
