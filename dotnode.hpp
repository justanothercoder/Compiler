#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"
#include "structsymbol.hpp"
#include "referencetype.hpp"
#include "typehelper.hpp"
#include "globalhelper.hpp"
#include "codegen.hpp"

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, string member_name);   
    ~DotNode();
    
    virtual void build_scope();
    virtual void check();
    virtual void gen();

    virtual Type* getType() const;
    virtual bool isLeftValue() const;
    
private:

    ExprNode *base;

    string member_name;

    StructSymbol *base_type;
    Symbol *member;
};

#endif
