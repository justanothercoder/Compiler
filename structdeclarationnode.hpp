#ifndef _STRUCTDECLARATIONNODE_HPP_
#define _STRUCTDECLARATIONNODE_HPP_

#include <vector>

#include "declarationnode.hpp"
#include "structsymbol.hpp"
#include "symboldefine.hpp"

using std::vector;

class StructDeclarationNode : public DeclarationNode
{
public:

    StructDeclarationNode(string name, const vector<AST*>& inner);

    virtual ~StructDeclarationNode();

    virtual AST* copyTree() const;
    
    void build_scope();    

    virtual Symbol* getDefinedSymbol() const;

	virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;
    
protected:

    string name;
    vector<AST*> inner;

    StructSymbol *definedSymbol;
};

#endif
