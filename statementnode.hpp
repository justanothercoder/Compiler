#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>
#include <algorithm>

#include "ast.hpp"
#include "codeobject.hpp"

class StatementNode : public AST
{   
public:

    StatementNode(std::vector<AST*> statements);

    virtual ~StatementNode();

    virtual AST* copyTree() const;
    
    CodeObject& gen() override;

	virtual std::vector<AST*> getChildren() const;
	
	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

	std::vector<AST*> statements;
	CodeObject code_obj;    
};

#endif
