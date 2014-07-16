#ifndef _NUMBERNODE_HPP_
#define _NUMBERNODE_HPP_

#include "exprnode.hpp"
#include "globalconfig.hpp"
#include "builtins.hpp"
#include "codeobject.hpp"

class NumberNode : public ExprNode
{
public:

    NumberNode(string num);

    virtual void check();
    virtual CodeObject& gen();

    virtual AST* copyTree() const;

	string getNum() const;
    
	virtual VariableType getType() const;
	virtual bool isLeftValue() const;

	virtual void freeTempSpace();
	
	bool isCompileTimeExpr() const override;
	optional<int> getCompileTimeValue() const override;

private:

    string num;
	CodeObject code_obj;    
};
    
#endif
