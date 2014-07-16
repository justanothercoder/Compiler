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
	~DotNode() override;
    
    void check() override;
    CodeObject& gen() override;

	vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    VariableType getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	optional<int> getCompileTimeValue() const override;

private:

    ExprNode *base;

    string member_name;

    StructSymbol *base_type;
    VariableSymbol *member;
	CodeObject* code_obj;
};

#endif
