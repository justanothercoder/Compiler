#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "typehelper.hpp"
#include "globalhelper.hpp"
#include "codegen.hpp"
#include "templatestructsymbol.hpp"

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, string member_name);   

	virtual ~DotNode();
    
    virtual AST* copyTree() const;

    virtual VariableType getType() const;

    virtual void check(const TemplateInfo& template_info);
    virtual void gen(const TemplateInfo& template_info);    

	virtual vector<AST*> getChildren() const;
    
private:

    ExprNode *base;

    string member_name;

    StructSymbol *base_type;
    VariableSymbol *member;
};

#endif
