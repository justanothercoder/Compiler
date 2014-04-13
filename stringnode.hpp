#ifndef _STRINGNODE_HPP_
#define _STRINGNODE_HPP_

#include "exprnode.hpp"
#include "builtins.hpp"
#include "codeobject.hpp"

class StringNode : public ExprNode
{
public:

	StringNode(string str);

	virtual AST* copyTree() const;
	virtual VariableType getType() const;

    virtual void check(const TemplateInfo& template_info);
    virtual CodeObject& gen(const TemplateInfo& template_info);

	virtual std::vector<AST*> getChildren() const;

	string getStr() const;

	static string getNewLabel();

private:
	
	string str;
	CodeObject code_obj;
};

#endif
