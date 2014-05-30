#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "typehelper.hpp"
#include "globalhelper.hpp"
#include "codeobject.hpp"
#include "templatestructsymbol.hpp"
#include "codeobject.hpp"

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, string member_name);   
	virtual ~DotNode();
    
    virtual void check();
    virtual CodeObject& gen();    

	virtual vector<AST*> getChildren() const;
    virtual AST* copyTree() const;

    virtual VariableType getType() const;
	virtual bool isLeftValue() const;

private:

    ExprNode *base;

    string member_name;

    StructSymbol *base_type;
    VariableSymbol *member;
	CodeObject* code_obj;
};

#endif
