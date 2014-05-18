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

    virtual void check(const TemplateInfo& template_info);
    virtual CodeObject& gen(const TemplateInfo& template_info);

    virtual AST* copyTree() const;

	string getNum() const;
    
	virtual VariableType getType() const;
	virtual bool isLeftValue() const;

private:

    string num;
	CodeObject code_obj;    
};
    
#endif
