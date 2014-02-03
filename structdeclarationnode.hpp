#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "structsymbol.hpp"

using std::vector;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(string name, const vector<DeclarationNode*>& inner);

    virtual ~StructDeclarationNode();
    
    virtual void build_scope();    
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual Symbol* getDefinedSymbol() const;

    virtual void template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual bool isTemplated() const;

    const vector<DeclarationNode*>& getInner() const;
    
protected:

    string name;
    vector<DeclarationNode*> inner;

    StructSymbol *definedSymbol;
};

#endif
