#ifndef _STRINGNODE_HPP_
#define _STRINGNODE_HPP_

#include "exprnode.hpp"
#include "builtins.hpp"
#include "codeobject.hpp"

class StringNode : public ExprNode
{
public:

	StringNode(string str);

    virtual void check();
    virtual CodeObject& gen();

	virtual AST* copyTree() const;

	string getStr() const;
	static string getNewLabel();

	virtual VariableType getType() const;
	virtual bool isLeftValue() const;

	virtual int neededSpaceForTemporaries();

private:
	
	string str;
	CodeObject code_obj;
};

#endif
